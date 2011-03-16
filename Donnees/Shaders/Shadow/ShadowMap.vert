// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

invariant gl_Position;

void main ()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0);
}
