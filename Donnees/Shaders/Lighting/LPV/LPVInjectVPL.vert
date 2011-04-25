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
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim
uniform int CurrentLevel;

// Sortie shader
smooth out vec2 inTexCoord;
smooth out vec3 inNormal;

//invariant gl_Position;

void main()
{	
	// Add Texcoords
	inTexCoord.x = VertexPosition.x;
	inTexCoord.y = VertexPosition.y;

	// Get data from GBuffers
	vec3 Position = PositionFormDepth(DepthBuffer, inTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, inTexCoord).xyz * 2.0 - 1.0);
	inNormal = Normal.xyz; ///< Need to compute the SH coeff

	gl_Position = vec4(Position, 1.0);
}
