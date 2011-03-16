#version 130

// Precision qualifier
precision highp float;

uniform sampler2D ShadowBuffer;
uniform bool DebugMode;

// Entree
smooth in vec4 outShadowCoord;

// Sortie
out vec4 Color;

float LinearizeDepth(float zoverw){
	float n = 1.0; // camera z near
	float f = 40.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{	
	vec4 shadowCoordinateWdivide = outShadowCoord / outShadowCoord.w ;
	
    // Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z -= 0.001;
	
	float distanceFromLight = texture(ShadowBuffer, shadowCoordinateWdivide.st).r;
	
	
 	float shadow = 1.0;
 	if (outShadowCoord.w > 0.0) // Pas dans le depth buffer
 	{
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
 		Color = shadow * vec4(1.0);
	}
	else
	{
		Color = vec4(1.0,0.0,0.0,1.0);
	}
	
	if(DebugMode)
	{
		distanceFromLight = LinearizeDepth(distanceFromLight);
		Color = vec4(distanceFromLight,distanceFromLight,distanceFromLight,1.0);
	}
	
}