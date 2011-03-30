#version 130

// Precision qualifier
precision highp float;

uniform sampler2D FluxBuffer;

smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

void main()
{	
	// Put into buffers
	Color = texture2D(FluxBuffer,outTexCoord);

}
