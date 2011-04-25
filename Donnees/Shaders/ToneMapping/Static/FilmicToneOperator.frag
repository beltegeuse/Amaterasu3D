#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D HDRBuffer;

// Output buffer
out vec4 Result;

// Take from http://filmicgames.com/archives/75
// This tone operator is used in Uncharted 2
// Thanks John Hable for his amazing work

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 Uncharted2Tonemap(vec3 x)
{
   return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}


void main()
{
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;

	//HDRValue *= 16;  // Hardcoded Exposure Adjustment

	float ExposureBias = 1.0;
	vec3 curr = Uncharted2Tonemap(ExposureBias*HDRValue);

	vec3 whiteScale = 1.0/Uncharted2Tonemap(vec3(W));
	vec3 color = curr*whiteScale;

	vec3 retColor = pow(color,vec3(1/2.2));//

	Result = vec4(retColor,1.0);
}
