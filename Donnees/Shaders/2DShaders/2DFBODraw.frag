#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D TextureDiffuse;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

void main()
{	
	Color = texture(TextureDiffuse, outTexCoord);
}