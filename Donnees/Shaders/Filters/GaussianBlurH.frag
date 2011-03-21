#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D TextureBuffer;

// Output buffer
out vec4 Result;

float Width = 800;
float Coeff[9] = float[](0.0677841, 0.0954044, 0.121786, 0.140999, 0.148054, 0.140999, 0.121786, 0.0954044, 0.0677841);
void main()
{
	float InvCoordH = 1.0 / Width;
	vec4 Sum = vec4(0.0);
	for(int i = -4; i < 4; i++)
	{
		Sum += Coeff[i+4]*texture(TextureBuffer,outTexCoord + vec2(i*InvCoordH,0));
	}

	Result = Sum;
}
