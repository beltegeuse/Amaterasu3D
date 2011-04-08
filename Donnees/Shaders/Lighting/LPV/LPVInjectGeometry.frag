#version 130

// Precision qualifier
precision highp float;

// Parametres
uniform vec4 LPVCellSize;

// Entree
smooth in vec3 outNormal;
smooth in float SurfelArea;

// Sortie
out vec4 Grid;

#include <LPVSH.shadercode>

void main()
{	
	// Compute SH
	vec4 SH = SHProjectCone(normalize(outNormal.xyz));

	Grid = SH * clamp(SurfelArea,0.0,1.0);
}
