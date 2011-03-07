#version 130

// Precision qualifier
precision highp float;

// Entree
smooth in vec3 Color_in;

// Sortie
//out vec4 Color;

void main()
{	
	//vec3 ColorNorm = normalize(Color_in);
	//gl_FragColor = vec4 (ColorNorm,1.0);
	gl_FragColor = vec4 (Color_in,1.0);
}