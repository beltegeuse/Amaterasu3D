#version 130

#define DO_OCCLUSION
// Precision qualifier
precision highp float;

// Textures
uniform sampler2D LPVRed;
uniform sampler2D LPVGreen;
uniform sampler2D LPVBlue;
uniform sampler2D Occlusion;

// Parametre Grid
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

uniform bool DoOcclusion;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

//solid Angles
// 0.4006696846f / 4Pi;
//#define directFaceSubtendedSolidAngle 0.03188428
// 0.4234413544f / 4Pi;
//#define sideFaceSubtendedSolidAngle 0.03369559

#include <LPVPosition.shadercode>
#include <LPVSH.shadercode>

// Solid Angles
#define directFaceSubtendedSolidAngle 0.12753712
#define sideFaceSubtendedSolidAngle 0.13478556

// TODO: Hard coded : CAUSTION
// To know direction vectors
//5 / sqrt(125)
#define side1 0.447213595
//10 / sqrt(125)
#define side2 0.894427190

const vec3 SideDirection[4] = vec3[4](vec3(side1,0.0,side2), vec3(-side1,0.0,side2), vec3(0.0,side1,side2), vec3(0.0,-side1,side2));
const vec3 ReproDirection[4] = vec3[4](vec3(1.0,0.0,0.0), vec3(-1.0,0.0,0.0), vec3(0.0,1.0,0.0), vec3(0.0,-1.0,0.0));
const mat3 TransformationMatrix[6] = mat3[6](mat3( 1.0, 0.0, 0.0,
        									       0.0, 1.0, 0.0, // +Z
                                                   0.0, 0.0, 1.0),
                                             mat3(-1.0, 0.0, 0.0,
												   0.0, 1.0, 0.0, // -Z
												   0.0, 0.0,-1.0),
										     mat3( 0.0, 0.0, 1.0,
												   0.0, 1.0, 0.0, // +X
												   -1.0, 0.0, 0.0),
											 mat3( 0.0, 0.0,-1.0,
												   0.0, 1.0, 0.0, // -X
												   1.0, 0.0, 0.0),
										     mat3( 1.0, 0.0, 0.0,
												   0.0, 0.0, 1.0, // +Y
												   0.0,-1.0, 0.0),
										     mat3( 1.0, 0.0, 0.0,
												   0.0, 0.0,-1.0, // -Y
												   0.0, 1.0, 0.0));

const float Normalisation = 3.9;

void IVPropagateDir(inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue, in vec3 girdPos, in mat3 transMat)
{

	// *** Compute main direction
	vec3 offset = vec3(0.0,0.0,1.0)*transMat;
	// *** Get Neighbour caracteristics
	vec2 NeighbourTexCoord = Convert3DTo2DTexcoord(girdPos+offset); // TODO: Check border !!!!!
	vec4 NeighbourRed = texture(LPVRed,NeighbourTexCoord);
	vec4 NeighbourGreen = texture(LPVGreen,NeighbourTexCoord);
	vec4 NeighbourBlue = texture(LPVBlue,NeighbourTexCoord);
	vec4 occlusionSH = texture(Occlusion,NeighbourTexCoord);

	/*
	 * Main Direction (to front side)
	 */
	vec4 MainDirectionHemi = SHProjectCone90(offset);
	vec4 MainDirectionSH = SH_evaluate(offset * Normalisation);
	// *** Compute all flux
	float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
	float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
	float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));
	// *** Compute Fuzzy blocking
	float occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-offset),occlusionSH),1.0);
    //occlusionFactor *= occlusionFactor;
	// *** Add the contribution
	outputRed += MainDirectionHemi*fluxRed*directFaceSubtendedSolidAngle*occlusionFactor;
	outputGreen += MainDirectionHemi*fluxGreen*directFaceSubtendedSolidAngle*occlusionFactor;
	outputBlue += MainDirectionHemi*fluxBlue*directFaceSubtendedSolidAngle*occlusionFactor;

	/*
	 * Side Direction
	 */
	vec3 SideDirectionCompute;
	vec3 ReproDirectionCompute;
	vec4 SideDirectionHemi;
	vec4 SideDirectionSH;
	for(int i = 0; i < 4; i++)
	{
		// *** Compute directions
		SideDirectionCompute = SideDirection[i]*transMat;
		ReproDirectionCompute = ReproDirection[i]*transMat;
		// *** Compute SH Hemi for the direction
		SideDirectionHemi = SHProjectCone90(ReproDirectionCompute);
		SideDirectionSH = SH_evaluate(SideDirectionCompute * Normalisation);
		// *** Compute Flux
		fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
		fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
		fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
		// *** Compute Fuzzy blocking
		occlusionFactor =  1.0 - min( SHDotAbs(SH_evaluate(-SideDirectionCompute),occlusionSH),1.0);
		// *** Add the contribution
		outputRed += SideDirectionHemi*fluxRed*sideFaceSubtendedSolidAngle*occlusionFactor;
		outputGreen += SideDirectionHemi*fluxGreen*sideFaceSubtendedSolidAngle*occlusionFactor;
		outputBlue += SideDirectionHemi*fluxBlue*sideFaceSubtendedSolidAngle*occlusionFactor;
	}
}

void main(){
  vec4 resultRed = vec4(0.0,0.0,0.0,0.0);
  vec4 resultGreen = vec4(0.0,0.0,0.0,0.0);
  vec4 resultBlue = vec4(0.0,0.0,0.0,0.0);

  vec3 RealGridPosition = Convert2DTexcoordTo3D(outTexCoord);

  // Compute propagate Dir for all 6 neighbours
  for(int i = 0; i < 6; i++)
  {
	  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, TransformationMatrix[i]);
  }


  //Data for next propagation step
  GridRed = resultRed;
  GridGreen = resultGreen;
  GridBlue = resultBlue;
}
