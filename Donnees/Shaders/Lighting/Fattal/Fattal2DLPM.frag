#version 150

// Precision qualifier
precision highp float;

// In 
//in float DeltaData;
in vec2 vOriPosition;
in vec2 vOriDirection;
in float vOriValue;

// Out 
out vec4 outDeltaIBuffer;
out vec4 outDeltaUBuffer;

void main()
{
	outDeltaIBuffer = vec4(1.0); // vec4(DeltaData);
	outDeltaUBuffer = vec4(1.0); // vec4(DeltaData/4.0);
}
