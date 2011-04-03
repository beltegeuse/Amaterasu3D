#version 130

// Precision qualifier
precision highp float;

// Uniform data
uniform vec4 SpecularValue;

// Uniform States
uniform int UseBumpMapping;
uniform int UseDiffuseTex;
uniform int UseSpecularTex;

// Texture unit
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;

// Shader input
smooth in vec2 outTexCoord;
smooth in mat3 outtbnMatrix;
smooth in vec3 outColor;
smooth in vec3 outNormal;

// Material
uniform vec4 MaterialDiffuseColor;

// Shader output
out vec4 Normal;
out vec4 Flux;

// light caracteristics
uniform vec3 LightPosition;
uniform vec4 LightColor;
uniform vec3 LightSpotDirection;
uniform float LightRaduis;
uniform float LightIntensity;
uniform float LightCutOff; // cos value

#define M_PI 3.14159265358979323846264

void main()
{
    // Diffuse buffer
    vec4 diffuseColor = MaterialDiffuseColor;
    if(UseDiffuseTex == 1)
    {
		diffuseColor *= texture(TextureDiffuse, outTexCoord.st);
	}
    else if(outColor != vec3(0.0)) // FIXME: Impossible couleur noire ???
	{
	    diffuseColor = vec4(outColor,1.0);
	}
	
	// Normals buffer
	if(UseBumpMapping == 1)
	{
		vec4 normalMap = texture(TextureNormal, outTexCoord.st);
		vec3 n = normalize(2.0 * normalMap.rgb - 1.0); // decode the Bump texture
		vec3 normalInterpoled = n * outtbnMatrix;
		Normal = vec4(normalize(normalInterpoled)* 0.5 + 0.5,1.0);
	}
	else
	{
	    Normal = vec4(normalize(outNormal)* 0.5 + 0.5,1.0);
	}
	
	//
	vec3 LightDirection = LightPosition - outPosition;
	float LightDistance = length(LightDirection);
	LightDirection = normalize(LightDirection);
	float SpotDot = dot(normalize(LightSpotDirection), -LightDirection);
	
	Flux = vec4(0.0);
	if(LightDistance > LightRaduis || SpotDot < LightCutOff)
	{
	    // Nothing to do
	}
	else
	{
		// Compute light attenation
	    float SpotAtt = pow(SpotDot, 9.0); //TODO: uniform ???
	    float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;

		float NdotL = max(dot(vec3(Normal), LightDirection), 0.0);
		if (NdotL > 0.0) {
			// Add diffuse compoment
			Flux += vec4(LightAtt * (LightColor.rgb),1.0);
		}
		
		Flux *= diffuseColor;
	}
}
