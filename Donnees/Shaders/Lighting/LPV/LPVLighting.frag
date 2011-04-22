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
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim
uniform bool EnableTrilinearInterpolation;

// Parameters to compute position form Depth
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

#include <GetPosition.shadercode>
#include <LPVTrilinear.shadercode>

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

#include <LPVSH.shadercode>
#include <LPVPosition.shadercode>
#include <LPVTrilinear.shadercode>

void main()
{	
	// Get data from buffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 GridPosition = floor((Position.xyz - LPVPosition[0])/ vec3(LPVCellSize[0])); // Early discard
	if(IsNotInGrid(GridPosition))
	{
		discard;
	}

	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	vec4 DiffuseColor = texture(DiffuseBuffer,outTexCoord);

	vec4 CoeffGridRed;
	vec4 CoeffGridGreen;
	vec4 CoeffGridBlue;

	if(EnableTrilinearInterpolation)
	{
		CoeffGridRed = TrilinearInterpolationWorld(GridRed,Position, Normal);
		CoeffGridGreen = TrilinearInterpolationWorld(GridGreen,Position,Normal);
		CoeffGridBlue = TrilinearInterpolationWorld(GridBlue,Position, Normal);
	}
	else
	{

		// Get texture coordinates
		vec2 TexCoordGrid = Convert3DTo2DTexcoord(GridPosition);
		CoeffGridRed = texture2D(GridRed, TexCoordGrid); ///< And get coeff value
		CoeffGridGreen = texture2D(GridGreen, TexCoordGrid);
		CoeffGridBlue = texture2D(GridBlue, TexCoordGrid);
	}

	vec4 SHEv = SH_evaluate(-Normal);
	Color =  vec4(max(dot(CoeffGridRed,SHEv) / 3.145,0.0),
				max(dot(CoeffGridGreen,SHEv) / 3.145,0.0),
				max(dot(CoeffGridBlue,SHEv) / 3.145 ,0.0),1.0); //
	//Color = CoeffGrid;
}
