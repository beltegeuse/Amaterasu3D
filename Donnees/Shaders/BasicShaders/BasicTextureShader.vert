// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec2 VertexTexCoord;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

// Sortie shader
smooth out vec2 outTexCoord;

invariant gl_Position;

void main ()
{
	outTexCoord = VertexTexCoord;
	gl_Position = ProjectionMatrix * ModelMatrix * ViewMatrix * vec4 (VertexPosition, 1.0);
}
