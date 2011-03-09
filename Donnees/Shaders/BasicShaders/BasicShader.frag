#version 130

// Precision qualifier
precision highp float;

// Entree
smooth in vec3 Color_in;

// Sortie
out vec4 Color;

void main()
{	
	Color = vec4 (Color_in,1.0);
}