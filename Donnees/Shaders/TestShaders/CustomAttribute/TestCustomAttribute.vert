#version 150

// Precision qualifier
precision highp float;

// In attributes
in vec2 inPosition;

// Uniform
uniform vec2 ScreenSize;

void main()
{
	// put gl_Position normal
	gl_Position = vec4(inPosition/ScreenSize,0.0,1.0);
}
