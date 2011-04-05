#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;

in vec2 VertexPosition;

// Parameters to compute position from depth
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;
#include <GetPosition.shadercode>

// grids Parametres
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

// Sortie shader
smooth out vec3 outNormal;
smooth out float SurfelArea;
invariant gl_Position;
float perspectiveModifier = 1.0;

#include <LPVPosition.shadercode>

void main()
{	
	vec2 outTexCoord;
	outTexCoord.x = VertexPosition.x;
	outTexCoord.y = VertexPosition.y;

	// Get data
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	float Depth = DepthToZPosition(texture(DepthBuffer, outTexCoord).r);

	SurfelArea = (4.0*Depth*Depth) / (512.0*512.0); // Passer les parameters

	//shift occlusion volume by half cell size
	Position -= (LPVCellSize.xyz*0.5);

	// Prevent self shadowing
	//Position -= (Normal*LPVCellSize.xyz*0.5);

	Position = floor((Position-LPVPosition) / LPVCellSize.xyz);

	outNormal = Normal;
	vec2 pos2d = Convert3Dto2D(Position);
	gl_Position = vec4(pos2d*2.0-1.0,0.0,1.0);
}
