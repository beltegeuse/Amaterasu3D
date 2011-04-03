#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;

in vec2 VertexPosition;

// Parametres
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

// To compute position
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;
#include <GetPosition.shadercode>

// Sortie shader
smooth out vec2 outTexCoord;
smooth out vec3 outNormal;
invariant gl_Position;

#include <LPVPosition.shadercode>

void main()
{	
	outTexCoord.x = VertexPosition.x;
	outTexCoord.y = VertexPosition.y;

	// Get data
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	Position += (Normal*LPVCellSize.xyz*0.5);

	Position = floor((Position-LPVPosition) / LPVCellSize.xyz);

	outNormal = Normal.xyz;
	vec2 pos2d = Convert3Dto2D(Position);
	gl_Position = vec4(pos2d*2.0-1.0,0.0,1.0);
}
