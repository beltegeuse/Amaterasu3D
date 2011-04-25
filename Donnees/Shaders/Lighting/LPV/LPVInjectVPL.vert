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

// Sortie shader
flat out vec2 inTexCoord;
flat out vec3 inNormal;

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
