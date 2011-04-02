#version 130

// Precision qualifier
precision highp float;

//Les differents buffers
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;
uniform sampler2D RandomBuffer;
uniform sampler2D RendererBuffer;

// To deduce Position
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

#include <GetPosition.shadercode>

// Entree
smooth in vec2 outTexCoord;

// Output buffer
out vec4 AmbiantOcculsion;

// SSAO config
float SampleRaduis = 1.0;
float Scale = 1.0f;
float Bias = 0.0f;
float Intensity = 1.0f;

float calcAO(vec2 uv, vec2 coord, vec3 pos, vec3 norm)
{
	vec3 diff = PositionFormDepth(DepthBuffer, uv + coord).xyz - pos;
	vec3 v = normalize(diff);
	float d = length(diff) * Scale;
	return max(0.0, dot(norm, v) - Bias) * (1.0 / (1.0 + d)) * Intensity;
}

// Based on : http://www.gamerendering.com/2009/01/14/ssao/
// And : http://encelo.netsons.org/blog/tag/glsl/
void main()
{
	float Depth = texture(DepthBuffer, outTexCoord).r;
	if(Depth == 1.0)
		discard;

	// Samples vectors
	vec2 vec[4];
	vec[0] = vec2(1, 0);
	vec[1] = vec2(-1, 0);
	vec[2] = vec2(0, 1);
	vec[3] = vec2(0, -1);

	vec2 Rand = normalize(texture(RandomBuffer, vec2(600,800) * outTexCoord / 64.0).xy* 2.0 - 1.0);
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz* 2.0 - 1.0);
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;

	float AOFactor = 0.0;
	float Raduis = SampleRaduis / (Depth*100.0);

	int NbIteration = int(mix(4.0, 1.0, Depth));
	for(int i = 0; i < NbIteration; i++)
	{
		vec2 coord1 = reflect(vec[i], Rand) * Raduis;
		vec2 coord2 = vec2(coord1.x * 0.707 - coord1.y * 0.707, coord1.x * 0.707 + coord1.y * 0.707);

		AOFactor += calcAO(outTexCoord, coord1 * 0.25, Position, Normal);
		AOFactor += calcAO(outTexCoord, coord2 * 0.5, Position, Normal);
		AOFactor += calcAO(outTexCoord, coord1 * 0.75, Position, Normal);
		AOFactor += calcAO(outTexCoord, coord2, Position, Normal);
	}

	AOFactor /= float(NbIteration) * 4.0;
	AOFactor = 1.0 - clamp(AOFactor,0.0,1.0);
	AmbiantOcculsion = vec4(AOFactor,AOFactor,AOFactor,1.0);
}
