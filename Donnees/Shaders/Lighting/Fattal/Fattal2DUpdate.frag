#version 130

// Precision qualifier
precision highp float;

// In
smooth in vec2 outTexCoord;

// Texture
uniform sampler2D DeltaBuffer;
uniform sampler2D UBuffer;
uniform sampler2D IBuffer;

// Information
uniform vec2 MainDirection;

// Out buffers
out vec4 outUBuffer;
out vec4 outIBuffer;

////////////////////////
// Helper functions
////////////////////////
{% include 'Helpers/MainDirection.shadercode' %}

void main()
{
	// Read data
	vec vDelta = texture(DeltaBuffer, outTexCoord);
	float vI = texture(IBuffer, outTexCoord).a;
	vec4 vU = texture(UBuffer, outTexCoord);
	
	// Update I
	outIBuffer = vec4(vI + vDelta.a);
	
	// * Add values
	//outUBuffer = UpdateU(vU, MainDirection) + vDeltaU/4.0;
}
