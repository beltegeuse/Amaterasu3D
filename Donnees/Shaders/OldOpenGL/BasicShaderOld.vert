// VERTEX SHADER
#version 130

// Uniform
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

// Out
smooth out vec4 Color;

invariant gl_Position;

void main ()
{
	Color = gl_Color;
	//gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
}
