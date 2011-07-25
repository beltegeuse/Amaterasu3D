#version 150

// Precision qualifier
precision highp float;

// In 
flat in vec4 DeltaData;

// Out 
out vec4 outDeltaIBuffer;
out vec4 outDeltaUBuffer;

void main()
{
	outDeltaIBuffer = vec4(DeltaData);
	outDeltaUBuffer = vec4(DeltaData/4.0);
}
