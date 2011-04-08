#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D GridRed;
uniform sampler2D GridGreen; ///< Reprensent SH Grid
uniform sampler2D GridBlue;

uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DiffuseBuffer;

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

#include <LPVSH.shadercode>
#include <LPVPosition.shadercode>

// Trilinear interpolation
// \position : It's grid position
//TODO: Add border support
vec4 TrilinearInterpolation(sampler2D s, vec3 Position)
{
	vec3 IndiceCase = floor(Position);
	vec3 Offset = Position - IndiceCase;

	vec4 i1 = texture(s, Convert3DTo2DTexcoord(IndiceCase))*(1-Offset.z)+texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(0,0,1)))*Offset.z;
	vec4 i2 = texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(0,1,0)))*(1-Offset.z)+texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(0,1,1)))*Offset.z;
	vec4 j1 = texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(1,0,0)))*(1-Offset.z)+texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(1,0,1)))*Offset.z;
	vec4 j2 = texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(1,1,0)))*(1-Offset.z)+texture(s, Convert3DTo2DTexcoord(IndiceCase+vec3(1,1,1)))*Offset.z;

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
	// Get data from buffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	vec4 DiffuseColor = texture(DiffuseBuffer,outTexCoord);

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
		ComputeGridCoord(Position);

		// Get texture coordinates
		vec2 TexCoordGrid = Convert3DTo2DTexcoord(Position);
		CoeffGridRed = texture2D(GridRed, TexCoordGrid); ///< And get coeff value
		CoeffGridGreen = texture2D(GridGreen, TexCoordGrid);
		CoeffGridBlue = texture2D(GridBlue, TexCoordGrid);
	}

	vec4 SHEv = SH_evaluate(-Normal);
	Color = vec4(dot(CoeffGridRed,SHEv),
				 dot(CoeffGridGreen,SHEv),
				 dot(CoeffGridBlue,SHEv),1.0); //DiffuseColor

	//Color = CoeffGrid;
}
