#version 130

// Precision qualifier
precision highp float;

// Entree
smooth in vec3 outNormal;
smooth in float SurfelArea;

// Sortie
out vec4 Grid;

#include <LPVSH.shadercode>

void main()
{	
	// Compute SH
	vec4 SH = SHCreateHemi(normalize(-outNormal.xyz));
	//vec4 SH = vec4(1.0);

	Grid = SH;
	//Grid = vec4(0.0);
}
