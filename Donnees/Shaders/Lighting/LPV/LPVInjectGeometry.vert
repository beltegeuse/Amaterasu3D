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
	SurfelArea = Depth*Depth / (512.0*512.0); // TODO: Add uniform size of the buffers

	//shift occlusion volume by half cell size
	//Position -= (LPVCellSize.xyz*0.5);

	ComputeGridCoord(Position);
//	if(IsInGrid(Position))
//	{
		vec2 pos2d = Convert3DTo2DTexcoord(Position);
		gl_Position = vec4(pos2d*2.0-1.0,0.0,1.0);
//	}
//	else /* the point isn't in the grid */
//	{
//		// TODO: See if there is discard for vertex ????
//		gl_Position = vec4(-10.0,-10.0,0.0,1.0); // Put outside clipping plane
//	}
}
