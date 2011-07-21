#version 130

// Precision qualifier
precision highp float;

// Texture
uniform sampler2d Databuffer;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

void main()
{	
	Color = texture(Databuffer, outTexCoord).a;
}
