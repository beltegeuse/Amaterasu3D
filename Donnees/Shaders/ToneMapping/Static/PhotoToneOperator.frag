#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;
uniform sampler2D AdaptationBuffer;

// Output buffer
out vec4 Result;


const vec3 RGB2Lum = vec3(0.212671,0.71516,0.072169);

void main()
{
	// Read data
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;
	float Lw = dot(HDRValue,RGB2Lum);
	float La = texelFetch(AdaptationBuffer,ivec2(0),0).r;
	//float La = dot(texture(AdaptationBuffer,outTexCoord).xyz,RGB2Lum);
	//La = 10;

	// Compression
	float Lm = (0.18/La)*Lw;
	float Ld = Lm / ( Lm + 1 );

	Result = vec4(vec3(Ld)*pow(HDRValue/vec3(Lw), vec3(0.65)),1.0);
}
