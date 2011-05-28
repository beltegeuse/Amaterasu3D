#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;

// Output buffer
out float Result;

// Matlab proposition
//const vec3 RGB2Lum = vec3(0.2552  1.0834  0);
// Standards value in D65
const vec3 RGB2Lum = vec3(0.212671,0.71516,0.072169);

void main()
{
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;

	Result = dot(HDRValue,RGB2Lum);
}
