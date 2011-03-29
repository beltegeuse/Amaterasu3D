#version 130

// Precision qualifier
precision highp float;

// Entree
smooth in vec4 Value;
// Sortie
out vec4 Color;

void main()
{	
	// Put into buffers
	Color = Value;

}
