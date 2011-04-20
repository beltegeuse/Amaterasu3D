#version 130
#extension GL_EXT_geometry_shader4 : enable

uniform float LPVNbLevels;

void main(void)
{

	//increment variable
	int i;

	for(i=0; i< gl_VerticesIn; i++){
		gl_Position = gl_PositionIn[i];
		EmitVertex();
	}
	EndPrimitive();

}
