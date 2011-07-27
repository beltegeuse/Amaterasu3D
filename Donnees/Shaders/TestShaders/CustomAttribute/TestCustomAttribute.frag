#version 150

// Precision qualifier
precision highp float;

// In
in vec3 outColor;

// Out 
out vec4 Color;

void main()
{
	Color = vec4(outColor, 1.0);
}
