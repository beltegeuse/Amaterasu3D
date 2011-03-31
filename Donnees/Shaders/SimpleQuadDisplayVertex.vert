// VERTEX SHADER
#version 130

// Sortie shader
smooth out vec2 outTexCoord;

in vec2 VertexPosition;

invariant gl_Position;

void main ()
{
	outTexCoord.x = max(0,VertexPosition.x);
	outTexCoord.y = max(0,VertexPosition.y);
	gl_Position = gl_Vertex;
}
