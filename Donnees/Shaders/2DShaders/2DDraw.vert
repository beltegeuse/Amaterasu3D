// VERTEX SHADER
#version 130

// Sortie shader
smooth out vec2 outTexCoord;

// Attributes
in vec2 VertexTexCoord;
in vec2 VertexPosition;

// Matrix
uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;

invariant gl_Position;

void main ()
{
	outTexCoord = VertexTexCoord;
	gl_Position = ProjectionMatrix*ModelMatrix*vec4(VertexPosition,0,1);
}
