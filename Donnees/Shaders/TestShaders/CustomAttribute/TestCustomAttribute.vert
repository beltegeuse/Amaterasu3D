#version 150

// Precision qualifier
precision highp float;

// In attributes
in vec2 inPosition;
in vec3 inColor;

// Out
out vec3 outColor;

// Uniform
uniform vec2 ScreenSize;

void main()
{
	outColor = inColor;
	// put gl_Position normal
	gl_Position = vec4((inPosition/ScreenSize) * 2 - 1,0.0,1.0);
}
