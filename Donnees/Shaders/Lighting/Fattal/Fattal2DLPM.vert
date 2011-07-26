#version 150

// Precision qualifier
precision highp float;

// In attributes
in vec2 OriPosition;
in vec2 OriDirection;
in float OriValue;

// Out 
out vec2 vOriPosition;
out vec2 vOriDirection;
out float vOriValue;

// Uniform
uniform vec2 GridDimension;
uniform vec2 MainDirection;

void main()
{
	// Pass Attributes to Geometry shader
	vOriPosition = OriPosition;
	vOriDirection = normalize(OriDirection);
	vOriValue = OriValue;
	
	// put gl_Position normal
	gl_Position = vec4(0.0,0.0,0.0,1.0);
}
