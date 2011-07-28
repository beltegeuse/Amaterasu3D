#version 150

// Precision qualifier
precision highp float;

// In 
in float DeltaData;

// Out 
out vec4 outDeltaIBuffer;
//out vec4 outDeltaUBuffer;

void main()
{
	outDeltaIBuffer = vec4(DeltaData); // DeltaData
	//outDeltaUBuffer = vec4(DeltaData/4.0); //DeltaData
}
