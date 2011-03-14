// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec2 VertexTexCoord;

// Smooth variables
smooth out vec2 outTexCoord;

invariant gl_Position;

void main ()
{
	outTexCoord = VertexTexCoord;
	gl_Position = vec4 (VertexPosition, 1.0);
}
