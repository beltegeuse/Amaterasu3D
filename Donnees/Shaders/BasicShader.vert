// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

invariant gl_Position;

void main ()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (VertexPosition, 1.0);
}
