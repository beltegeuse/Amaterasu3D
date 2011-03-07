#version 130

// Precision qualifier
precision highp float;

// Entree
smooth in vec3 Color_in;

// Sortie
//out vec4 Color;

void main()
{	
	vec3 ColorNorm = normalize(Color_in);
	gl_FragColor = vec4 (1.0,0,0,1.0);
}