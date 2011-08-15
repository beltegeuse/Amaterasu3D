#version 150

// Precision qualifier
precision highp float;

// In 
flat in float DeltaData;

// Out 
out vec4 outDeltaBuffer;

void main()
{
	outDeltaBuffer = vec4(DeltaData);
}
