#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;

layout(points, max_vertices = NB_CASCADE) out;

// grids Parametres
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim

uniform vec3 ObsPosition;

#include <LPVPosition.shadercode>

smooth in vec3 inNormal[1];

smooth out vec3 outNormal;
smooth out float SurfelArea;

void main()
{

	//increment variable
	int i;
	int j;
	vec3 Position;
	vec3 CellID;
	vec2 Pos2D;
	for(i=0; i< gl_VerticesIn; i++){
		for(j=0; j < NB_CASCADE; j++)
		{
			outNormal = inNormal[i];
			float depth = length(ObsPosition-Position);
			SurfelArea = (4.0*depth*depth)/(512.0*512.0);

			Position = gl_PositionIn[i].xyz;
			// Prevent self shadowing
			//Position += (outNormal*LPVCellSize[j]*0.5);
			// Compute the cell ID
			if(LPVCellSize[j] == 0)
				break;
			CellID = floor(((Position.xyz - LPVPosition[j].xyz) / vec3(LPVCellSize[j])) - vec3(0.5));
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
