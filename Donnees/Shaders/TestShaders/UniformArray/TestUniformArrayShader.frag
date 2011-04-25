#version 130

// Precision qualifier
precision highp float;

out vec4 Color;

void main()
{
	//Yeah, yeah, yeah ... we just color the pixel
	// this example is showing off geometry shaders, not fragments!
	//Shade to blue!
	Color = vec4(0.0,0.0,1.0,1.0);
}
