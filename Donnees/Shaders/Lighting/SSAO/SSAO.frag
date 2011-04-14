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
float SampleRaduis = 0.006f;
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
	vec2 vectors[10] = vec2[](vec2(-0.010735935, 0.01647018),vec2(-0.06533369, 0.3647007),vec2(-0.6539235, -0.016726388),vec2(0.40958285, 0.0052428036),vec2(-0.1465366, 0.09899267),vec2(-0.44122112, -0.5458797),vec2(0.03755566, -0.10961345),vec2(0.019100213, 0.29652783),vec2(0.8765323, 0.011236004),vec2(0.29264435, -0.40794238));
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz* 2.0 - 1.0);
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;

	float jitterAngle= texture2D(RandomBuffer, outTexCoord.st * 10.0).x*2.0*3.14;
	mat2 jitterMatrix= mat2(cos(jitterAngle), sin(jitterAngle), -sin(jitterAngle), cos(jitterAngle));

	float AOFactor = 0.0;
	float Raduis = SampleRaduis / Depth;

	int NbIteration = 10;
	for(int i = 0; i < NbIteration; i++)
	{
		vec2 offset = vectors[i]*jitterMatrix* Raduis;
		AOFactor += calcAO(outTexCoord, offset, Position, Normal);
	}

	AOFactor = 1.0 - clamp(AOFactor*0.1,0.0,1.0);
	AmbiantOcculsion = vec4(AOFactor,AOFactor,AOFactor,1.0);
}
