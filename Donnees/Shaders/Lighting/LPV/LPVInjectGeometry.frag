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
	vec4 SH = SHProjectCone(normalize(outNormal.xyz)); // FIXME

	Grid = 2.0 * SH * (SurfelArea / (LPVCellSize.x*LPVCellSize.y));
//	Grid = vec4((SurfelArea / (LPVCellSize.x*LPVCellSize.y)));
//	Grid = vec4(outNormal.xyz,1.0);
//	Grid = vec4(,1.0);
}
