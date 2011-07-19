#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;

// Output buffer
out vec4 Result;

#define LN_10 2.30258509

// Matlab proposition
//const vec3 RGB2Lum = vec3(0.2552  1.0834  0);
// Standards value in D65
const vec3 RGB2Lum = vec3(0.212671,0.71516,0.072169);

{% include 'DisplayHelper.shadercode' %}

void main()
{
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;

	float Luminance = dot(HDRValue,RGB2Lum);

	// We only want value beetween [1, 10^5]
	float NormalizedValue = min(max(log(Luminance)/LN_10, 0.0), 5)*50;

	Result = vec4(matlabHotColormap256(NormalizedValue),1.0);
}
