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

// Output shader
smooth out vec3 inNormal;

//invariant gl_Position;

void main()
{	

	// Compute tex coords
	vec2 outTexCoord;
	outTexCoord.x = VertexPosition.x;
	outTexCoord.y = VertexPosition.y;

	// Get data from Buffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	//float Depth = DepthToZPosition(texture(DepthBuffer, outTexCoord).r);
	inNormal = Normal;

	// Compute Surfel area
	//SurfelArea = 12000.0 * Depth*Depth / (512.0*512.0); // TODO: Add uniform size of the buffers
	//float depthFactor = (Depth - NearClipping) / (FarClipping - NearClipping);
	//SurfelArea = 0.206 * (depthFactor * depthFactor);
	//SurfelArea = 1.0;

	gl_Position = vec4(Position,1.0);


}
