#version 130

// Precision qualifier
precision highp float;

//Les differents buffers
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;
uniform sampler2D RandomBuffer;
uniform sampler2D RendererBuffer;

// Entree
smooth in vec2 outTexCoord;

// Output buffer
out vec4 Color;

// SSAO config
float totStrength = 1.38;
float strength = 0.07;
float falloff = 0.000002;
float TextureOffset = 18.0;
float Raduis = 0.006;

#define SAMPLES 16 // 10 is good
const float invSamples = 1.0/16.0;


// Based on : http://www.gamerendering.com/2009/01/14/ssao/
void main()
{
	// 16 samples form an sphere
	vec3 pSphere[16] = vec3[](vec3(0.53812504, 0.18565957, -0.43192),vec3(0.13790712, 0.24864247, 0.44301823),vec3(0.33715037, 0.56794053, -0.005789503),vec3(-0.6999805, -0.04511441, -0.0019965635),vec3(0.06896307, -0.15983082, -0.85477847),vec3(0.056099437, 0.006954967, -0.1843352),vec3(-0.014653638, 0.14027752, 0.0762037),vec3(0.010019933, -0.1924225, -0.034443386),vec3(-0.35775623, -0.5301969, -0.43581226),vec3(-0.3169221, 0.106360726, 0.015860917),vec3(0.010350345, -0.58698344, 0.0046293875),vec3(-0.08972908, -0.49408212, 0.3287904),vec3(0.7119986, -0.0154690035, -0.09183723),vec3(-0.053382345, 0.059675813, -0.5411899),vec3(0.035267662, -0.063188605, 0.54602677),vec3(-0.47761092, 0.2847911, -0.0271716));

	// Get the Random Normal vector
	vec3 RandomNormal = normalize((texture(RandomBuffer,outTexCoord*TextureOffset).xyz*2.0) - 1.0);

	// Get information from fragment
	vec3 CurrentNormal = texture(NormalBuffer,outTexCoord).xyz*2.0 - vec3(1.0);
	float CurrentDepth = texture(DepthBuffer,outTexCoord).r;
	vec3 CurrentPositionSS = vec3(outTexCoord,CurrentDepth);

	// Adapt the size of the half sphere
	float RaduisNormalized = Raduis; // / CurrentDepth; //FIXME: Depth non linear ??? => fix this

	// All informations to compute SSAO
	float OccFactor = 0.0;
	for(int i=0; i<SAMPLES;++i) // Compute SSAO
	{
		// Get an random vector
		vec3 Ray = RaduisNormalized*reflect(pSphere[i],RandomNormal);

		// Get the occluder fragment
		vec2 TexCoordOccluder = outTexCoord + dot(Ray,CurrentNormal)*Ray.xy;//sign(
		vec3 Occluder = texture(NormalBuffer, TexCoordOccluder).xyz;
		float OccluderDepth = texture(NormalBuffer, TexCoordOccluder).r;
		float DepthDiff = CurrentDepth - OccluderDepth;

		OccFactor +=  step(falloff,DepthDiff)*(1.0-dot(Occluder.xyz,CurrentNormal))*(1.0-smoothstep(falloff,strength,DepthDiff));
	}

	// output the result
	float SSAOFactor = clamp(1.0-totStrength*OccFactor*invSamples,0.0,1.0);
	Color = vec4(SSAOFactor*texture(RendererBuffer,outTexCoord).xyz,1.0);


}
