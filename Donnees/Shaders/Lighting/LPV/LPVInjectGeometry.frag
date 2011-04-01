#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FluxBuffer;

// Parametres
float CellSize = 10.0;

// Entree
smooth in float SurfelArea;
smooth in vec3 outNormal;

// Sortie
out vec4 Grid;

#include <LPVSH.shadercode>

void main()
{	

	// Compute SH
	vec4 SH = SHProjectCone(normalize(outNormal)); // FIXME

	Grid = SH; //SurfelArea

}
