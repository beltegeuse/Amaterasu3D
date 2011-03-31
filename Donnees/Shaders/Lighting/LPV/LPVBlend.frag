#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D LPVRed;
uniform sampler2D LPVGreen; ///< Reprensent SH Grid
uniform sampler2D LPVBlue;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

void main()
{
	GridRed = texture(LPVRed,outTexCoord);
	GridGreen = texture(LPVGreen,outTexCoord);
	GridBlue = texture(LPVBlue,outTexCoord);
}
