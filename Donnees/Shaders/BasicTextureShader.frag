#version 130

// Precision qualifier
precision highp float;

//Uniform
uniform sampler2D Texture0;

// Entree
smooth in vec2 outTexCoord;

// Sortie
//out vec4 Color;

void main()
{	
	gl_FragColor = texture(Texture0, outTexCoord);
}