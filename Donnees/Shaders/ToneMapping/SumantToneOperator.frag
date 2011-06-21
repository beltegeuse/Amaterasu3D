#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture
uniform sampler2D HDRBuffer;

// Tone operator Input
uniform float SigCone;
uniform float SigRod;
uniform float Arod;
uniform float Acone;
uniform float Brod;
uniform float Bcone;

// Output buffer
out vec4 Result;

// To compute lumiances
const vec3 RGB2Ycone = vec3(0.256,0.67,0.065);
const vec3 RGB2Yrod = vec3(-0.702,1.039,0.433);

// Others constantes
//const float n = 0.73;
const float n = 1.0;
const float blkFactor = 5.0 / 32.0; 
const float whtFactor = 5.0;
const float Sd = 0.1383;

// Display constant
const float REFwhtDisplay = 125;
const float REFblkDisplay = 4;
const float QmidDisplay = 64.5;
const float QdiffDisplay = 121; 

float CompressionSig(float B, float Lum, float Sig)
{
	return B*pow(Lum,n) / (pow(Lum,n) + pow(Sig,n));
}

void main()
{
	// Read data
	vec3 HDRValue = texture(HDRBuffer,outTexCoord).xyz;
	float Lcone = max(dot(HDRValue, RGB2Ycone), 0.0);
	float Lrod = max(dot(HDRValue, RGB2Yrod), 0.0);
	
	////////////////////////////////////
	// Adaptation model (Compression)
	////////////////////////////////////
	
	float Rrod = CompressionSig(Brod, Lrod, SigRod);
	float Rcone = CompressionSig(Bcone, Lcone, SigCone);

	// Adaptation model output
	float Scolor = (n*Bcone*pow(Lcone,n)*pow(SigCone,n)) / pow(pow(Lcone,n) + pow(SigCone,n), 2);
	// * Two output of tone mapping
	vec3 Rcolor = pow(HDRValue / Lcone, vec3(Scolor));
	float Rlum = Rrod + Rcone;
	// * For logic display
	float REFblk = CompressionSig(Brod, Arod*blkFactor, SigRod) + CompressionSig(Bcone, Acone*blkFactor, SigCone);
	float REFwht = CompressionSig(Brod, Arod*whtFactor, SigRod) + CompressionSig(Bcone, Acone*whtFactor, SigCone);
	
	//////////////////////////////////
	// Appearance model
	//////////////////////////////////
	//Rlum -= REFblk;
	float Qmid = 0.5*(REFblk + REFwht);
	float Qdiff = REFwht - REFblk;

	//////////////////////////////////
	// Inverse Appearance model
	//////////////////////////////////
	
	// Impossible to deal without min and max
	// Display logic
	//if(Qdiff > QdiffDisplay)
	//{
	//	Rlum *= (Qdiff / QdiffDisplay);
	//}

    //////////////////////////////////
	// Inverse adaptation model
	//////////////////////////////////
	float Ld = pow(Rlum / ( 1 - Rlum), 1.0 / n); 

	Result = vec4(Ld*pow(Rcolor, vec3(1.0 / 0.1383)),1.0);
	//Result = vec4(pow(Rcolor, vec3(1.0 / 0.1383)) / 4.0,1.0);
	//Result = vec4(Rcolor,1.0);
	//Result = vec4(vec3(Scolor),1.0);
	//Result = vec4(vec3(Ld),1.0);
}	
