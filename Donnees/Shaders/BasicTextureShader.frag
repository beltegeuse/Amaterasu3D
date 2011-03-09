#version 130

// Precision qualifier
precision highp float;

//Uniform
uniform sampler2D Texture0;

// Entree
smooth in vec3 outTexCoord;

void main()
{	
	gl_FragColor = texture(Texture0, outTexCoord.xy);
}