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

#define 1_LOG10 0.434294482

float GetCoef(float v, float sig)
{
	return exp(-v/(2.0*sig*sig));
}

const vec3 RGB2Lum = vec3(0.3576,0.7152,0.1192); 

void main()
{
	ivec2 SizeTextureInt = floor(SizeTexture);
	ivec2 STexCoord = floor(SizeTexture*outTexCoord);
	
	vec3 Svalue = texelFetch(InputBuffer, STexCoord, 0);
	
	vec3 Pvalue;
	ivec2 PTexCoord;
	int i;
	int j;
	float K = 0.0;
	vec3 Sum = vec3(0.0);
	float Coeff = 0.0;
	
	for(i = -HalfSizeNeigbours; i < HalfSizeNeigbours; i++)
	{
		for(j = -HalfSizeNeigbours; j < HalfSizeNeigbours; j++)
		{
			PTexCoord = max(ivec2(0,0), min(SizeTextureInt, TexCoord+ivec2(i,j)));
			Pvalue = texelFetch(InputBuffer, PTexCoord, 0);
			
			Coeff = GetCoef(length(PTexCoord - STexCoord), SigS)*GetCoef(ln(abs(dot(Pvalue,RGB2Lum) -  dot(Svalue,RGB2Lum)))*1_LOG10, SigR);
			K += Coeff;
			Sum += Coeff*Pvalue;
		}
	}

	Result = vec4(Sum / K,1.0);
}
