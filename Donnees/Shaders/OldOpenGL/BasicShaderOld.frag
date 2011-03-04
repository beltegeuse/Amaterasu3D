#version 130

// Precision qualifier
precision highp float;

smooth in vec4 Color;

void main()
{	
	//gl_FragColor = vec4(1.0,0,0,1.0);
	gl_FragColor = Color;
}