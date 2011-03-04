// VERTEX SHADER
#version 130

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

// Out
smooth out vec4 Color;

invariant gl_Position;

void main ()
{
	Color = gl_Color;
	gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex; 
	//gl_Position = ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex; 
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex; 
}
