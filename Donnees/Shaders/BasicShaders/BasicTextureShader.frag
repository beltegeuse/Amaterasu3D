#version 130

// Precision qualifier
precision highp float;

//Uniform
uniform sampler2D Texture0;

// Entree
smooth in vec2 outTexCoord;

// Output buffers
out vec4 Color;

void main()
{	
	Color = texture(Texture0, outTexCoord);
}