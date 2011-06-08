#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;

// Output buffer
out vec4 Result;


const vec3 RGB2Lum = vec3(0.212671,0.71516,0.072169);

void main()
{
	// Read data
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;

	Result = vec4(vec3(dot(HDRValue, RGB2Lum)),1.0);
}
