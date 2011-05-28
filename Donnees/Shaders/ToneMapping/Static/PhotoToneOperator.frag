#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;
uniform sampler2D LuminanceBuffer;

// Output buffer
out vec4 Result;

void main()
{
	// Read data
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;
	float Lw = texture(LuminanceBuffer,outTexCoord).r;
	float La = texture(LuminanceBuffer,outTexCoord).r;

	// Compression
	float Lm = (0.18/La)*Lw;
	float Ld = Lm / ( Lm + 1 );



	Result = vec4(Ld,Ld,Ld,1.0);
}
