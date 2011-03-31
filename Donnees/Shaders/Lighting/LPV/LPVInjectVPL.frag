#version 130

//this is c = vec4(c0,-c1,c1,-c1)
//with c0 = 1 / ( 2 * sqrt(PI))
//     c1 = sqrt(3) / ( 2 * sqrt(PI))
#define SH_c vec4(0.282094792,-0.488602512,0.488602512,0.488602512)
//#define SH_cosLobe_c vec4(0.886226925,-1.02332671,1.02332671,1.02332671)
#define SH_cosLobe_c vec4(0.25,-0.5,0.5,0.5)
//#define SH_cosLobe_c vec4(0.4,-0.8,0.8,0.8)

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
out vec4 GridRed;
out vec4 GridGreen;
out vec4 GridBlue;

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

vec4 SHCreateHemi(in vec3 dir)
{
  //return SHRotate(dir, vec2(0.25,0.5));
  return SH_cosLobe_c * vec4(1.0,dir.y,dir.z,dir.x);
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
	vec4 SH = SHCreateHemi(normalize(outNormal)) / 5.0; // FIXME
	// Put into buffers
	//Grid = SH * Luminance(Flux);

	GridRed = SH * Flux.r;
	GridGreen = SH * Flux.g;
	GridBlue = SH * Flux.b;
}
