#version 130
#extension GL_EXT_gpu_shader4 : enable

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D LPVRed;
uniform sampler2D LPVGreen;
uniform sampler2D LPVBlue;
uniform sampler2D Occlusion;

// Parametre Grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform int LPVNbCell;// number cell in one dim
uniform bool DoOcclusion;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 GridBlue;
out vec4 GridGreen;
out vec4 GridRed;

//solid Angles
// 0.4006696846f / 4Pi;
//#define directFaceSubtendedSolidAngle 0.03188428
// 0.4234413544f / 4Pi;
//#define sideFaceSubtendedSolidAngle 0.03369559

{% include 'HelpersCode/LPVPosition.shadercode' %}
{% include 'HelpersCode/LPVSH.shadercode' %}

// Solid Angles
#define directFaceSubtendedSolidAngle 0.12753712
#define sideFaceSubtendedSolidAngle 0.13478556

vec2 GetSamplePos2DOffset3D(in vec2 coords, in vec3 offset, in int cascadeID)
{
	return Convert3DTo2DTexcoord(Convert2DTexcoordTo3D(coords, cascadeID)+offset, cascadeID);
}

vec4 Sample2DOffset3D(in sampler2D s, in vec2 coords, in vec3 offset, in int cascadeID){
	return texture(s,GetSamplePos2DOffset3D(coords,offset, cascadeID));
}

/// Normalized vector
//1 / sqrt(5)
#define side1 0.447213595
//2 / sqrt(5)
#define side2 0.894427190

const float RangeModifier = 1.0;

void propagate(in vec2 pos, in int cascadeID,in mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue)
{
	//evaluate main direction
	vec3 MainDirection =  vec3(0.0,0.0,1.0) * orientation;

	// If on border => Discard this gathering
	if(IsNotInGrid(Convert2DTexcoordTo3D(pos, cascadeID)-MainDirection))
		return;

	// Compute all coeffs
	vec4 MainDirectionSH = SH_evaluate(MainDirection * RangeModifier);
	vec4 MainDirectionHemi = SH_evaluateCosineLobe_direct(MainDirection);

	//Get neighbour SHs
	vec2 loadPos = GetSamplePos2DOffset3D(pos,-MainDirection, cascadeID);
	vec4 NeighbourRed = texture(LPVRed,loadPos);
	vec4 NeighbourGreen = texture(LPVGreen,loadPos);
	vec4 NeighbourBlue = texture(LPVBlue,loadPos);

	//Get Occlusion SH
	vec4 occlusionSH = vec4(0.0);
	if(DoOcclusion)
	{
		occlusionSH = Sample2DOffset3D(Occlusion,pos,+MainDirection * 0.5,cascadeID);
	}

	float occlusionFactor = 1.0;

	//Evaluate main face
	float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
	float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
	float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));
	if(DoOcclusion)
	{
		occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-MainDirection),occlusionSH),1.0);
		occlusionFactor *= occlusionFactor;
	}
	vec4 resultRed = MainDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
	vec4 resultGreen = MainDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
	vec4 resultBlue = MainDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;


	//evaluate side faces
	vec3 SideDirection, ReproDirection;
	vec4 SideDirectionSH, SideDirectionHemi;
	{% set coeff=[(1.0,0.0),(-1.0,0.0),(0.0,1.0),(0.0,-1.0)] %}
	{% for i in range(4) %}
		SideDirection = vec3(vec2({{coeff[i][0]}}, {{coeff[i][1]}}) *side1,side2) * orientation;
		ReproDirection = vec3(vec2({{coeff[i][0]}}, {{coeff[i][1]}}),0.0) * orientation;
		SideDirectionSH = SH_evaluate(SideDirection * RangeModifier);
		SideDirectionHemi = SH_evaluateCosineLobe_direct(ReproDirection);
		fluxRed = max(0.0,dot(SideDirectionSH,NeighbourRed));
		fluxGreen = max(0.0,dot(SideDirectionSH,NeighbourGreen));
		fluxBlue = max(0.0,dot(SideDirectionSH,NeighbourBlue));
		if(DoOcclusion)
		{
			occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-SideDirection),occlusionSH),1.0);
			occlusionFactor *= occlusionFactor;
		}
		resultRed += SideDirectionHemi * fluxRed * directFaceSubtendedSolidAngle * occlusionFactor;
		resultGreen += SideDirectionHemi * fluxGreen * directFaceSubtendedSolidAngle * occlusionFactor;
		resultBlue += SideDirectionHemi * fluxBlue * directFaceSubtendedSolidAngle * occlusionFactor;
	{% endfor %}

	outputRed += resultRed;
	outputGreen += resultGreen;
	outputBlue += resultBlue;
}

void main(){
	vec4 resultRed = vec4(0.0,0.0,0.0,0.0);
	vec4 resultGreen = vec4(0.0,0.0,0.0,0.0);
	vec4 resultBlue = vec4(0.0,0.0,0.0,0.0);
	
	////////////////
	// Compute the cascade
	///////////////
	int cascadeID = int(floor(outTexCoord.y * {{NB_CASCADE}}));
	
	//TODO: Deduire le niveau avec les coordonnes de textures
	//Z+
	propagate(outTexCoord,cascadeID,mat3( 1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0, 1.0),resultRed,resultGreen,resultBlue);
	//Z-
	propagate(outTexCoord,cascadeID,mat3(-1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0,-1.0),resultRed,resultGreen,resultBlue);
	//X+
	propagate(outTexCoord,cascadeID,mat3( 0.0, 0.0, 1.0,
								0.0, 1.0, 0.0,
								-1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
	//X-
	propagate(outTexCoord,cascadeID,mat3( 0.0, 0.0,-1.0,
								0.0, 1.0, 0.0,
								1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
	//Y+
	propagate(outTexCoord,cascadeID,mat3( 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0,
								0.0,-1.0, 0.0),resultRed,resultGreen,resultBlue);
	//Y-
	propagate(outTexCoord,cascadeID,mat3( 1.0, 0.0, 0.0,
								0.0, 0.0,-1.0,
								0.0, 1.0, 0.0),resultRed,resultGreen,resultBlue);

	//Data for next propagation step
	GridRed = resultRed;
	GridGreen = resultGreen;
	GridBlue = resultBlue;
}



////////////////////////////////////
/// OLD CODE

//void propagateLight(in vec2 pos, in mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue)
//{
//	vec3 MainDirection = vec3(0.0,0.0,1.0)*orientation;
//	vec4 MainDirectionSH = SH_evaluate(MainDirection);
//	vec4 MainDirectionHemi = SH_evaluateCosineLobe_direct(MainDirection);
//
//	ivec2 loadPos = GetLoadPos2DOffset3D(pos,-MainDirection);
//	vec4 NeighbourRed = texelFetch2D(LPVRed,loadPos, 0);
//	vec4 NeighbourGreen = texelFetch2D(LPVGreen,loadPos, 0);
//	vec4 NeighbourBlue = texelFetch2D(LPVBlue,loadPos, 0);
//
//	float occlusionFactor = 1.0;
//	if(DoOcclusion)
//	{
//		vec4 occlusionSH = Sample2DOffset3D(Occlusion,pos,-MainDirection);
//		occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-MainDirection),occlusionSH),1.0);
//		occlusionFactor *= occlusionFactor;
//	}
//
//	float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
//	float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
//	float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));
//
//	outputRed += MainDirectionHemi * fluxRed * occlusionFactor * 0.66667936;
//	outputGreen += MainDirectionHemi * fluxGreen * occlusionFactor * 0.66667936;
//	outputBlue += MainDirectionHemi * fluxBlue * occlusionFactor * 0.66667936;
//}

//vec4 Load2DOffset3D(in sampler2D s, in vec2 coords, in vec3 offset){
//	return texelFetch2D(s,GetLoadPos2DOffset3D(coords,offset),0);
//}



