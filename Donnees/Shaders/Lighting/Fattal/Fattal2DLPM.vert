#version 150

// Precision qualifier
precision highp float;

// In attributes
in vec2 OriPosition;
in vec2 OriDirection;

// Out 
out flat vec2 vOriPosition;
out flat vec2 vOriDirection;

void main()
{
	// Pass Attributes to Geometry shader
	vOriPosition = OriPosition;
	vOriDirection = OriDirection;
	
	// put gl_Position normal
	gl_Position = vec4(0.0,0.0,1.0,1.0);
}
