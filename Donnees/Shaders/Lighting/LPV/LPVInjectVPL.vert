#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;

in vec2 VertexPosition;

// Parametres
uniform mat4 LPVMatrix; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim
#include <LPVPosition.shadercode>

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

void main()
{	
	// Add Texcoords
	outTexCoord.x = VertexPosition.x;
	outTexCoord.y = VertexPosition.y;

	// Get data from GBuffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	outNormal = Normal.xyz; ///< Need to compute the SH coeff

	// Prevent self shadowing
	Position += (Normal*LPVCellSize.xyz*0.5);

	vec3 cell = floor((LPVMatrix*vec4(Position.xyz,1.0)).xyz / LPVCellSize.xyz);
    if(IsInGrid(cell))
    {
    	vec2 pos2d = Convert3Dto2D(cell);
    	pos2d /= LPVSize.xy;
    	gl_Position = vec4(pos2d * 2.0 - 1.0,0.0,1.0);
    }
    else
    	gl_Position = vec4(vec3(-3),1.0); // Hors pour clipping


}
