#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FluxBuffer;

// Entree
smooth in vec2 outTexCoord;
smooth in vec3 outNormal;
// Sortie
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

#include <LPVSH.shadercode>

void main()
{	
	// Get all data
	vec4 Flux = texture(FluxBuffer, outTexCoord)  / Pi; // TODO: Normalisation pb ????
	// Compute SH
	vec4 SH = SHProjectCone(normalize(outNormal));

	// Put into buffers
	GridRed = SH * Flux.r;
	GridGreen = SH * Flux.g;
	GridBlue = SH * Flux.b;
}
