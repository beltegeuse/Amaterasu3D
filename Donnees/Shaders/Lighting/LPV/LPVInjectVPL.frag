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

#define Pi 3.1415926f

void main()
{	
	// Get all data
	vec4 Flux = 0.1 * texture(FluxBuffer, outTexCoord); // TODO: Normalisation pb ????
	// Compute SH
	vec4 SH = SHCreateHemi(outNormal) / Pi;// ;
	//vec4 Flux = texture(FluxBuffer, outTexCoord); // * 1.0 / (512.0*512.0);
	// Put into buffers
	GridRed = SH * Flux.r;
	GridGreen = SH * Flux.g;
	GridBlue = SH * Flux.b;
}
