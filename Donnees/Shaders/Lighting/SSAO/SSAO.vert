// VERTEX SHADER
#version 130

// Sortie shader
smooth out vec2 outTexCoord;

invariant gl_Position;

void main ()
{
	outTexCoord.x = max(0,gl_Vertex.x);
	outTexCoord.y = max(0,gl_Vertex.y);
	gl_Position = gl_Vertex;
}
