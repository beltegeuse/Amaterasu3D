#version 130

//this is c = vec4(c0,-c1,c1,-c1)
//with c0 = 1 / ( 2 * sqrt(PI))
//     c1 = sqrt(3) / ( 2 * sqrt(PI))
#define SH_c vec4(0.282094792,-0.488602512,0.488602512,0.488602512)
//#define SH_cosLobe_c vec4(0.886226925,-1.02332671,1.02332671,1.02332671)
#define SH_cosLobe_c vec4(0.25,-0.5,0.5,0.5)
//#define SH_cosLobe_c vec4(0.4,-0.8,0.8,0.8)

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D GridRed;
uniform sampler2D GridGreen; ///< Reprensent SH Grid
uniform sampler2D GridBlue;

uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;

// Parametres
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim
uniform bool EnableTrilinearInterpolation;

// Parameters to compute position form Depth
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

#include <GetPosition.shadercode>

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

//Evaluates a SH
vec4 SH_evaluate(vec3 dir){
  return SH_c * vec4(1.0, dir.y, dir.z, dir.x);
}

#include <LPVPosition.shadercode>

// Trilinear interpolation
// \position : It's grid position
//TODO: Add border support
vec4 TrilinearInterpolation(sampler2D s, vec3 Position)
{
	vec3 IndiceCase = floor(Position);
	vec3 Offset = Position - IndiceCase;

	vec4 i1 = texture(s, Convert3Dto2D(IndiceCase))*(1-Offset.z)+texture(s, Convert3Dto2D(IndiceCase+vec3(0,0,1)))*Offset.z;
	vec4 i2 = texture(s, Convert3Dto2D(IndiceCase+vec3(0,1,0)))*(1-Offset.z)+texture(s, Convert3Dto2D(IndiceCase+vec3(0,1,1)))*Offset.z;
	vec4 j1 = texture(s, Convert3Dto2D(IndiceCase+vec3(1,0,0)))*(1-Offset.z)+texture(s, Convert3Dto2D(IndiceCase+vec3(1,0,1)))*Offset.z;
	vec4 j2 = texture(s, Convert3Dto2D(IndiceCase+vec3(1,1,0)))*(1-Offset.z)+texture(s, Convert3Dto2D(IndiceCase+vec3(1,1,1)))*Offset.z;

	vec4 w1 = i1*(1 - Offset.y) + i2 * Offset.y;
	vec4 w2 = j1*(1 - Offset.y) + j2 * Offset.y;

	return w1*(1 - Offset.x) + w2 * Offset.x;
}

vec4 TrilinearInterpolationWorld(sampler2D s, vec3 Position)
{
	vec3 PositionGrid = ((Position-LPVPosition) / LPVCellSize.xyz)-0.5;
	return TrilinearInterpolation(s,PositionGrid);
}

void main()
{	
	// Get data
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);

	vec4 CoeffGridRed;
	vec4 CoeffGridGreen;
	vec4 CoeffGridBlue;
	if(EnableTrilinearInterpolation)
	{
		CoeffGridRed = TrilinearInterpolationWorld(GridRed,Position);
		CoeffGridGreen = TrilinearInterpolationWorld(GridGreen,Position);
		CoeffGridBlue = TrilinearInterpolationWorld(GridBlue,Position);
	}
	else
	{
		// Get Grid Coordinates
		Position = floor((Position-LPVPosition) / LPVCellSize.xyz);

		// Get texture coordinates
		vec2 TexCoordGrid = Convert3Dto2D(Position);
		CoeffGridRed = texture2D(GridRed, TexCoordGrid); ///< And get coeff value
		CoeffGridGreen = texture2D(GridGreen, TexCoordGrid);
		CoeffGridBlue = texture2D(GridBlue, TexCoordGrid);
	}

	if(CoeffGridRed == vec4(0,0,0,1) && CoeffGridGreen == vec4(0,0,0,1) && CoeffGridBlue == vec4(0,0,0,1))
		Color = vec4(0.0);
	else
	{
		vec4 SHEv = SH_evaluate(-Normal);
		Color = vec4(dot(CoeffGridRed,SHEv),
				     dot(CoeffGridGreen,SHEv),
				     dot(CoeffGridBlue,SHEv),1.0);
	}
	//Color = CoeffGrid;
}
