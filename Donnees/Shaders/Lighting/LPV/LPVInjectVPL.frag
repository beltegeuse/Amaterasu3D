#version 130

//this is c = vec4(c0,-c1,c1,-c1)
//with c0 = 1 / ( 2 * sqrt(PI))
//     c1 = sqrt(3) / ( 2 * sqrt(PI))
#define SH_c vec4(0.282094792,-0.488602512,0.488602512,0.488602512)
//#define SH_cosLobe_c vec4(0.886226925,-1.02332671,1.02332671,1.02332671)
#define SH_cosLobe_c vec4(0.25,-0.5,0.5,0.5)
//#define SH_cosLobe_c vec4(0.4,-0.8,0.8,0.8)

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FluxBuffer;

// Parametres
float CellSize = 10.0;

// Entree
smooth in vec2 outTexCoord;
smooth in vec3 outNormal;
// Sortie
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

#include <LPVSH.shadercode>

float Luminance(vec4 color)
{
	return color.r*0.2126+color.g*0.7152+color.b*0.0722;
}

void main()
{	
	// Get all data
	vec4 Flux = texture(FluxBuffer, outTexCoord);
	// Compute SH
	vec4 SH = SHProjectCone(normalize(outNormal)) / 5.0; // FIXME
	// Put into buffers
	//Grid = SH * Luminance(Flux);

	GridRed = SH * Flux.r;
	GridGreen = SH * Flux.g;
	GridBlue = SH * Flux.b;
}
