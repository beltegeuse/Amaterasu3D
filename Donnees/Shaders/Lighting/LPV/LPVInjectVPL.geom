#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(points, max_vertices = NB_CASCADE)out;

// grids Parametres
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim

#include <LPVPosition.shadercode>

flat in vec2 inTexCoord[1];
flat in vec3 inNormal[1];

smooth out vec2 outTexCoord;
smooth out vec3 outNormal;

void main()
{

	//increment variable
	int i;
	int j;
	vec3 Position;
	vec3 CellID;
	vec2 Pos2D;
	for(i=0; i< gl_VerticesIn; i++){
		for(j=0; j< NB_CASCADE; j++)
		{
			outNormal = inNormal[i];
			outTexCoord = inTexCoord[i];

			Position = gl_PositionIn[i].xyz;
			// Prevent self shadowing
			Position += (outNormal*LPVCellSize[j]*0.5);
			// Compute the cell ID
			if(LPVCellSize[j] == 0)
				break;
			CellID = floor((Position.xyz - LPVPosition[j].xyz) / vec3(LPVCellSize[j]));
			if(IsInGrid(CellID))
			{
				Pos2D = Convert3DTo2DTexcoord(CellID,j);
				gl_Position = vec4(Pos2D * 2.0 - 1.0,0.0,1.0);
				EmitVertex();
			}
			else
				break; // Fast terminaison
		}
	}
	EndPrimitive();

}
