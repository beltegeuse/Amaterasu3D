#version 150

// Precision qualifier
precision highp float;

// In 
in flat vec4 DeltaData;

// Out 
out vec4 outDeltaIBuffer;
out vec4 outDeltaUBuffer;

void main()
{
	outDeltaIBuffer = DeltaData;
	outDeltaUBuffer = DeltaData;
}
