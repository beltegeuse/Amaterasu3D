#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FluxBuffer;

// Parametres
float CellSize = 10.0;

// Entree
smooth in vec2 outTexCoord;
smooth in vec3 outNormal;
// Sortie
out vec4 Grid;

// Pick from Paper : Light Propagation Volumes in CryEngine3
vec4 SHRotate(vec3 vcDir, vec2 vZHCoeffs)
{
	// Compute sine and cosine of theta angle
	// beware of singularity when both x and y are 0 (no rotation)
	vec2 theta12_cs = normalize(vcDir.xy);

	// Compute sine and cosine of phi angle
	vec2 phi12_cs;
	phi12_cs.x = sqrt(1.0 - vcDir.z * vcDir.z);
	phi12_cs.y = vcDir.z;

	vec4 vResult;
	// First band is rotation independant
	vResult.x = vZHCoeffs.x;
	// Rotating the second band SH
	vResult.y = vZHCoeffs.y * phi12_cs.x * theta12_cs.y;
	vResult.z = -vZHCoeffs.y * phi12_cs.y;
	vResult.w = vZHCoeffs.y * phi12_cs.x * theta12_cs.x;
	return vResult;
}

vec4 SHProjectCone(vec3 vcDir)
{
	vec2 vZHCoeffs = vec2(0.25,0.5);
	return SHRotate(vcDir, vZHCoeffs);
}

float Luminance(vec4 color)
{
	return color.r*0.2126+color.g*0.7152+color.b*0.0722;
}

void main()
{	
	// Get all data
	vec4 Flux = texture(FluxBuffer, outTexCoord);
	// Compute SH
	vec4 SH = SHProjectCone(normalize(outNormal));
	// Put into buffers
	Grid = SH * Luminance(Flux);

}
