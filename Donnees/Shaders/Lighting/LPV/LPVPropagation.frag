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
uniform mat4 LPVMatrice; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

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

#include <LPVPosition.shadercode>
#include <LPVSH.shadercode>

// Solid Angles
#define directFaceSubtendedSolidAngle 0.12753712
#define sideFaceSubtendedSolidAngle 0.13478556

ivec2 GetLoadPos2DOffset3D(in vec2 coords, in vec3 offset){
	vec2 c = floor(coords * LPVSize.xy);
	float row = floor(c.y / LPVCellSize.w);
	float col = floor(c.x / LPVCellSize.w);
	c = clamp(vec2(c.x - col * LPVCellSize.w,c.y - row * LPVCellSize.w) + offset.xy,0.0,LPVCellSize.w-1.0);
	col += offset.z;
	float coldiff = min(col,0.0) + max(0.0,col-LPVSize.z+1.0);
	float newrow = clamp(row+coldiff,0.0,LPVSize.w-1.0);
	col += (min(coldiff,0.0) * (-LPVSize.z) + max(coldiff,0.0) * (-LPVSize.z)) * abs(newrow - row);
	col = clamp(col,0.0,LPVSize.z-1.0);
	vec2 spos = vec2(col * LPVCellSize.w + c.x,newrow * LPVCellSize.w + c.y);// / LPVSize.xy;
	return ivec2(spos);
}

vec4 Load2DOffset3D(in sampler2D s, in vec2 coords, in vec3 offset){
	return texelFetch2D(s,GetLoadPos2DOffset3D(coords,offset),0);
}

vec2 GetSamplePos2DOffset3D(in vec2 coords, in vec3 offset){
	vec2 c = coords * LPVSize.xy;
	float row = floor(c.y / LPVCellSize.w);
	float col = floor(c.x / LPVCellSize.w);
	c = clamp(vec2(c.x - col * LPVCellSize.w,c.y - row * LPVCellSize.w) + offset.xy,0.0,LPVCellSize.w-1.0);
	col += offset.z;
	float coldiff = min(col,0.0) + max(0.0,col-LPVSize.z+1.0);
	float newrow = clamp(row+coldiff,0.0,LPVSize.w-1.0);
	col += (min(coldiff,0.0) * (-LPVSize.z) + max(coldiff,0.0) * (-LPVSize.z)) * abs(newrow - row);
	col = clamp(col,0.0,LPVSize.z-1.0);
	vec2 spos = vec2(col * LPVCellSize.w + c.x,newrow * LPVCellSize.w + c.y) / LPVSize.xy;
	return spos;
}

vec4 Sample2DOffset3D(in sampler2D s, in vec2 coords, in vec3 offset){
	return texture2D(s,GetSamplePos2DOffset3D(coords,offset));
}

//1 / sqrt(5)
#define side1 0.447213595
//2 / sqrt(5)
#define side2 0.894427190

const float RangeModifier = 8.0;

void propagateLight(in vec2 pos, in mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue)
{
	vec3 MainDirection = vec3(0.0,0.0,1.0)*orientation;
	vec4 MainDirectionSH = SH_evaluate(MainDirection);
	vec4 MainDirectionHemi = SH_evaluateCosineLobe_direct(MainDirection);

	ivec2 loadPos = GetLoadPos2DOffset3D(pos,-MainDirection);
	vec4 NeighbourRed = texelFetch2D(LPVRed,loadPos, 0);
	vec4 NeighbourGreen = texelFetch2D(LPVGreen,loadPos, 0);
	vec4 NeighbourBlue = texelFetch2D(LPVBlue,loadPos, 0);

	float occlusionFactor = 1.0;
	if(DoOcclusion)
	{
		vec4 occlusionSH = Sample2DOffset3D(Occlusion,pos,-MainDirection);
		occlusionFactor = 1.0 - min( SHDotAbs(SH_evaluate(-MainDirection),occlusionSH),1.0);
		occlusionFactor *= occlusionFactor;
	}

	float fluxRed = max(0.0,dot(MainDirectionSH,NeighbourRed));
	float fluxGreen = max(0.0,dot(MainDirectionSH,NeighbourGreen));
	float fluxBlue = max(0.0,dot(MainDirectionSH,NeighbourBlue));

	outputRed += MainDirectionHemi * fluxRed * occlusionFactor * 0.66667936;
	outputGreen += MainDirectionHemi * fluxGreen * occlusionFactor * 0.66667936;
	outputBlue += MainDirectionHemi * fluxBlue * occlusionFactor * 0.66667936;
}

const vec2 coeffs[4] = vec2[](vec2(1.0,0.0),vec2(-1.0,0.0),vec2(0.0,1.0),vec2(0.0,-1.0));

void propagate(in vec2 pos, in mat3 orientation, inout vec4 outputRed, inout vec4 outputGreen, inout vec4 outputBlue)
{
	//evaluate main direction
	vec3 MainDirection =  vec3(0.0,0.0,1.0) * orientation;
	vec4 MainDirectionSH = SH_evaluate(MainDirection * RangeModifier);
	vec4 MainDirectionHemi = SH_evaluateCosineLobe_direct(MainDirection);

	//Get neighbour SHs
	ivec2 loadPos = GetLoadPos2DOffset3D(pos,-MainDirection);
	vec4 NeighbourRed = texelFetch2D(LPVRed,loadPos, 0);
	vec4 NeighbourGreen = texelFetch2D(LPVGreen,loadPos, 0);
	vec4 NeighbourBlue = texelFetch2D(LPVBlue,loadPos, 0);

	//Get Occlusion SH
	vec4 occlusionSH;
	if(DoOcclusion)
	{
		occlusionSH = Sample2DOffset3D(Occlusion,pos,-MainDirection * 0.5);
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
	int i;
	vec3 SideDirection, ReproDirection;
	vec4 SideDirectionSH, SideDirectionHemi;
	for(i = 0; i < 4; i++)
	{

		SideDirection = vec3(coeffs[i]*side1,side2) * orientation;
		ReproDirection = vec3(coeffs[i],0.0) * orientation;
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
	}

	outputRed += resultRed;
	outputGreen += resultGreen;
	outputBlue += resultBlue;
}

void main(){
	vec4 resultRed = vec4(0.0,0.0,0.0,0.0);
	vec4 resultGreen = vec4(0.0,0.0,0.0,0.0);
	vec4 resultBlue = vec4(0.0,0.0,0.0,0.0);
	//Z+
	propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0, 1.0),resultRed,resultGreen,resultBlue);
	//Z-
	propagate(outTexCoord,mat3(-1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0,-1.0),resultRed,resultGreen,resultBlue);
	//X+
	propagate(outTexCoord,mat3( 0.0, 0.0, 1.0,
								0.0, 1.0, 0.0,
								-1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
	//X-
	propagate(outTexCoord,mat3( 0.0, 0.0,-1.0,
								0.0, 1.0, 0.0,
								1.0, 0.0, 0.0),resultRed,resultGreen,resultBlue);
	//Y+
	propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0,
								0.0,-1.0, 0.0),resultRed,resultGreen,resultBlue);
	//Y-
	propagate(outTexCoord,mat3( 1.0, 0.0, 0.0,
								0.0, 0.0,-1.0,
								0.0, 1.0, 0.0),resultRed,resultGreen,resultBlue);

	//Data for next propagation step
	GridRed = resultRed;
	GridGreen = resultGreen;
	GridBlue = resultBlue;
}
