// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexColor;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

// Smooth variables
smooth out vec3 Color_in;

invariant gl_Position;

void main ()
{
	Color_in = VertexColor;
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (VertexPosition, 1.0);
}
