#version 130

// Sortie shader
smooth out vec2 outTexCoord;

invariant gl_Position;

void main ()
{
	outTexCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}
