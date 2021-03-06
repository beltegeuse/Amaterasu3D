#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture
uniform sampler2D InputBuffer;

// Constantes
uniform vec2 SizeTexture;
uniform int HalfSizeNeigbours;
uniform float SigS;
uniform float SigR;

// Output buffer
out vec4 Result;

#include <MathHelper.shadercode>

float GetCoef(float v, float sig)
{
	return exp(-(v*v)/(2.0*sig*sig));
}

const vec3 RGB2Lum = vec3(0.3576,0.7152,0.1192); 

void main()
{
	ivec2 SizeTextureInt = ivec2(floor(SizeTexture));
	ivec2 STexCoord = ivec2(floor(SizeTexture*outTexCoord));
	
	vec3 Svalue = texelFetch(InputBuffer, STexCoord, 0).rgb;
	
	vec3 Pvalue;
	ivec2 PTexCoord;
	int i;
	int j;
	float K = 0.0;
	vec3 Sum = vec3(0.0);
	float Coeff = 0.0;
	float dist = 0.0;
	
	for(i = -HalfSizeNeigbours; i < HalfSizeNeigbours; i++)
	{
		for(j = -HalfSizeNeigbours; j < HalfSizeNeigbours; j++)
		{
			PTexCoord = max(ivec2(0,0), min(SizeTextureInt, STexCoord+ivec2(i,j)));
			Pvalue = texelFetch(InputBuffer, PTexCoord, 0).rgb;
			dist = abs(dot(Pvalue,RGB2Lum) -  dot(Svalue,RGB2Lum));		
			Coeff = GetCoef(length(PTexCoord - STexCoord), SigS)*GetCoef(log10(dist), SigR);
			K += Coeff;
			Sum += Coeff*Pvalue;
			//Sum += Pvalue;
		}
	}

	Result = vec4(Sum / K,1.0);
	//Result = vec4(Svalue,1.0);
	//Result = vec4(Sum / (HalfSizeNeigbours*HalfSizeNeigbours*4),1.0);
}
