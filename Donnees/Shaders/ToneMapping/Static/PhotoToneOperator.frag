#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;
uniform sampler2D AdaptationBuffer;
uniform float AdaptationLum;

// Output buffer
out vec4 Result;


const vec3 RGB2Lum = vec3(0.212671,0.71516,0.072169);

void main()
{
	// Read data
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;
	float Lw = dot(HDRValue,RGB2Lum);

	// Compression
	float Lm = (0.18/AdaptationLum)*Lw;
	float Ld = Lm / ( Lm + 1 );

	Result = vec4(vec3(Ld)*pow(HDRValue/vec3(Lw), vec3(0.65)),1.0);
}
