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
uniform mat4 LPVMatrix; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

// Output shader
smooth out vec3 outNormal;
smooth out float SurfelArea;

invariant gl_Position;

#include <LPVPosition.shadercode>

void main()
{	

	// Compute tex coords
	vec2 outTexCoord;
	outTexCoord.x = VertexPosition.x;
	outTexCoord.y = VertexPosition.y;

	// Get data from Buffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	float Depth = DepthToZPosition(texture(DepthBuffer, outTexCoord).r);
	outNormal = Normal;

	// Compute Surfel area
	//SurfelArea = 12000.0 * Depth*Depth / (512.0*512.0); // TODO: Add uniform size of the buffers
	//float depthFactor = (Depth - NearClipping) / (FarClipping - NearClipping);
	//SurfelArea = 0.206 * (depthFactor * depthFactor);
	SurfelArea = 0.06;
	// Prevent self shadowing
	Position -= (Normal*LPVCellSize.xyz*0.5);
	//shift occlusion volume by half cell size
	//Position -= (LPVCellSize.xyz*0.5);

	vec3 cell = floor((LPVMatrix*vec4(Position.xyz,1.0)).xyz / LPVCellSize.xyz);
	vec2 pos2d = Convert3Dto2D(cell);
	pos2d /= LPVSize.xy;
	gl_Position = vec4(pos2d * 2.0 - 1.0,0.0,1.0);
}
