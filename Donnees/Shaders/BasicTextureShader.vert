// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexTexCoord;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

// Sortie shader
smooth out vec3 outTexCoord;

invariant gl_Position;

void main ()
{
	outTexCoord = VertexTexCoord;
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (VertexPosition, 1.0);
}
