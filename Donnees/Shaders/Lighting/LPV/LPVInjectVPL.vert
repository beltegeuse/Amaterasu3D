#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;

in vec2 VertexPosition;

// Parametres
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim
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
	Position += (Normal*LPVCellSize[0]*0.5);

	vec3 cell = floor((Position.xyz - LPVPosition[0].xyz) / vec3(LPVCellSize[0]));
    if(IsInGrid(cell))
    {
    	vec2 pos2d = Convert3Dto2D(cell,0);
    	pos2d /= LPVSize.xy;
    	gl_Position = vec4(pos2d * 2.0 - 1.0,0.0,1.0);
    }
    else
    	gl_Position = vec4(vec3(-3),1.0); // Hors pour clipping


}
