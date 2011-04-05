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

#define directFaceSubtendedSolidAngle 0.12753712
#define sideFaceSubtendedSolidAngle 0.13478556

#include <LPVPosition.shadercode>
#include <LPVSH.shadercode>

// TODO: Hard coded : CAUSTION
// To know direction vectors
//5 / sqrt(125)
#define side1 0.447213595
//10 / sqrt(125)
#define side2 0.894427190

//void propagate(vec2 pos, mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue)
//{
//  /*
//   * Compute all DATA
//   * Texture access and SH representations
//   */
//  // Get the Main direction
//  vec3 MainDirection = orientation * vec3(0.0,0.0,1.0);
//  vec4 MainDirectionSH = SH_evaluate(MainDirection);
//  vec4 MainDirectionHemi = SHProjectCone(MainDirection);
//
//  // Get the grid coordinates
//
//  vec2 loadPos = Convert3DTo2DTexcoord(RealGridPosition-MainDirection);
//
//  //Get SH value from the neighbour
//  vec4 NeighbourRed = texture2D(LPVRed,loadPos);
//  vec4 NeighbourGreen = texture2D(LPVGreen,loadPos);
//  vec4 NeighbourBlue = texture2D(LPVBlue,loadPos);
//
//
//  //Get Occlusion SH
//  vec4 occlusionSH = vec4(1.0);
//  if(DoOcclusion)
//  {
//	  occlusionSH = texture(Occlusion,loadPos);
//  }
//  float occlusionFactor = 1.0;
//
//  /*
//   * Evaluation of the Main face
//   */
//  float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
//  float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
//  float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));
//
//  if(DoOcclusion)
//  {
//	  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-MainDirection),occlusionSH),1.0);
//	  occlusionFactor *= occlusionFactor;
//  }
//  vec4 resultRed = MainDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
//  vec4 resultGreen = MainDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
//  vec4 resultBlue = MainDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
//
//
//  //evaluate side faces
//
//  vec3 SideDirection = vec3(side1,0.0,side2) * orientation;
//  vec3 ReproDirection = vec3(1.0,0.0,0.0) * orientation;
//
//  vec4 SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
//  vec4 SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
//  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
//  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
//  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
//  if(DoOcclusion)
//  {
//	  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
//	  occlusionFactor *= occlusionFactor;
//	  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
//  }
//  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
//
//  SideDirection = vec3(-side1,0.0,side2) * orientation;
//  ReproDirection = vec3(-1.0,0.0,0.0) * orientation;
//  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
//  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
//  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
//  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
//  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
//  if(DoOcclusion)
//  {
//	  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
//  	  occlusionFactor *= occlusionFactor;
//  	  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
//  }
//  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
//
//  SideDirection = vec3(0.0,side1,side2) * orientation;
//  ReproDirection = vec3(0.0,1.0,0.0) * orientation;
//  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
//  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
//  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
//  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
//  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
//  if(DoOcclusion)
//  {
//	  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
//	  occlusionFactor *= occlusionFactor;
//	  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
//  }
//  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
//
//  SideDirection = vec3(0.0,-side1,side2) * orientation;
//  ReproDirection = vec3(0.0,-1.0,0.0) * orientation;
//  SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
//  SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
//  fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
//  fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
//  fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
//  if(DoOcclusion)
//  {
//	  occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
//	  occlusionFactor *= occlusionFactor;
//	  //occlusionFactor = 1.0 - clamp( dot(SH_evaluate(-SideDirection),occlusionSH),0.0,1.0);
//  }
//  resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
//  resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
//
//  outputRed += resultRed;
//  outputGreen += resultGreen;
//  outputBlue += resultBlue;
//}

void IVPropagateDir(inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue, in vec3 girdPos, in vec3 offset)
{
	vec2 NeighbourTexCoord = Convert3DTo2DTexcoord(girdPos+offset); // TODO: Check border !!!!!

	vec4 NeighbourRed = texture2D(LPVRed,NeighbourTexCoord);
	vec4 NeighbourGreen = texture2D(LPVGreen,NeighbourTexCoord);
	vec4 NeighbourBlue = texture2D(LPVBlue,NeighbourTexCoord);

	vec4 MainDirectionHemi = SHProjectCone90(-offset);

	float fluxRed = max(0.0,dot(MainDirectionHemi,NeighbourRed));
	float fluxGreen = max(0.0,dot(MainDirectionHemi,NeighbourGreen));
	float fluxBlue = max(0.0,dot(MainDirectionHemi,NeighbourBlue));

	outputRed += MainDirectionHemi*fluxRed*directFaceSubtendedSolidAngle;
	outputGreen += MainDirectionHemi*fluxGreen*directFaceSubtendedSolidAngle;
	outputBlue += MainDirectionHemi*fluxBlue*directFaceSubtendedSolidAngle;
}

void main(){
  vec4 resultRed = vec4(0.0,0.0,0.0,0.0);
  vec4 resultGreen = vec4(0.0,0.0,0.0,0.0);
  vec4 resultBlue = vec4(0.0,0.0,0.0,0.0);

  vec3 RealGridPosition = Convert2DTexcoordTo3D(outTexCoord);

  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(1,0,0));
  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(-1,0,0));
  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(0,1,0));
  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(0,-1,0));
  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(0,0,1));
  IVPropagateDir(resultRed, resultGreen, resultBlue, RealGridPosition, vec3(0,0,-1));


  //Data for next propagation step
  GridRed = resultRed*3.9;
  GridGreen = resultGreen*3.9;
  GridBlue = resultBlue*3.9;
}
