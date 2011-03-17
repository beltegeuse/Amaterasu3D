#version 130

// Precision qualifier
precision highp float;
precision highp int;

//Les differents buffers
uniform sampler2D DiffuseBuffer;
uniform sampler2D SpecularBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D PositionBuffer;
uniform sampler2D DepthRSM;
uniform sampler2D NormalRSM;
uniform sampler2D PositionRSM;
uniform sampler2D FluxRSM;
uniform sampler2D NoiseBuffer;

// light caracteristics
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform vec3 LightSpotDirection;
uniform float LightRaduis;
uniform float LightIntensity;
uniform float LightCutOff; // cos value

// Shadow map
uniform mat4 LightViewMatrix;
uniform mat4 LightProjectionMatrix;

// To enable / disable the debug mode
uniform bool DebugMode;

// Entree
smooth in vec2 outTexCoord;

// Output buffers
out vec4 Color;

#define M_PI 3.14159265358979323846264

float LinearizeDepth(float zoverw){
	float n = 1.0; // camera z near
	float f = 100.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

/*
 * To create offsets of one texel and one half texel in the
 * texture lookup, we need to know the texture image size.
 */
#define ONE 0.00390625
#define ONEHALF 0.001953125

/*
 * The 5th degree smooth interpolation function for Perlin "improved noise".
 */
float fade(const float t)
{
  // return t*t*(3.0-2.0*t); // Old fade, yields discontinuous second derivative
  return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}

/*
 * 2D classic Perlin noise. Fast, but less useful than 3D noise.
 */
float noise(vec2 P)
{
  vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
  vec2 Pf = fract(P);             // Fractional part for interpolation

  // Noise contribution from lower left corner
  vec2 grad00 = texture(NoiseBuffer, Pi).rg * 4.0 - 1.0;
  float n00 = dot(grad00, Pf);

  // Noise contribution from lower right corner
  vec2 grad10 = texture(NoiseBuffer, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
  float n10 = dot(grad10, Pf - vec2(1.0, 0.0));

  // Noise contribution from upper left corner
  vec2 grad01 = texture(NoiseBuffer, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
  float n01 = dot(grad01, Pf - vec2(0.0, 1.0));

  // Noise contribution from upper right corner
  vec2 grad11 = texture(NoiseBuffer, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
  float n11 = dot(grad11, Pf - vec2(1.0, 1.0));

  // Blend contributions along x
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(Pf.x));

  // Blend contributions along y
  float n_xy = mix(n_x.x, n_x.y, fade(Pf.y));

  // We're done, return the final noise value.
  return n_xy;
}

vec4 rand(vec2 co)
{
	float timer = 1.0;
float noiseR = (fract(sin(dot(co ,vec2(12.9898,78.233))+timer) * 43758.5453));
float noiseG = (fract(sin(dot(co ,vec2(12.9898,78.233)*2)+timer) * 43758.5453));
float noiseB = (fract(sin(dot(co ,vec2(12.9898,78.233)*3)+timer) * 43758.5453));
return vec4(noiseR,noiseG,noiseB,1.0)*0.3;
}

void main()
{		
	// Get all data
	vec3 diffuseColor = texture(DiffuseBuffer, outTexCoord).xyz;
	vec4 specularColor = texture(SpecularBuffer, outTexCoord);
	vec3 normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	vec3 position = texture(PositionBuffer, outTexCoord).xyz;
	
	// If the light don't affect this frag => Discard
	vec3 LightDirection = LightPosition - position; // suppres realLightPosition
	float LightDistance = length(LightDirection);
	LightDirection = normalize(LightDirection);
	float SpotDot = dot(normalize(LightSpotDirection), -LightDirection);
	if (DebugMode == false)
	{
		if(LightDistance > LightRaduis || SpotDot < LightCutOff)
			discard;
	}
	
	// Shadow Map
	//  * Compute projected coordinates
	mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
						   0.0, 0.5, 0.0, 0.0,
						   0.0, 0.0, 0.5, 0.0,
						   0.5, 0.5, 0.5, 1.0);
	vec4 ShadowCoord = biasMatrix*LightProjectionMatrix * LightViewMatrix * vec4(position,1.0);
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z -= 0.001;
	
	float ClosedLightDistance = texture(DepthRSM, shadowCoordinateWdivide.st).r;
	float ShadowFactor = ClosedLightDistance < shadowCoordinateWdivide.z ? 0.2 : 1.0 ;
	
	
	// Compute light attenation
    float SpotAtt = pow(SpotDot, 12.0); //TODO: uniform ???
	float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;
	
	// =========== Compute Direct illumination
	// Initialise Black color
	Color = vec4(0.0);
	
	// Verify the light and the normal is in the same direction
	float NdotL = max(dot(normal, LightDirection), 0.0);
	if (NdotL > 0.0) {
		// Add diffuse compoment
		Color += vec4(LightAtt * (LightColor * NdotL),1.0);
		// Compute reflect vector
		//vec3 R = reflect(-LightDirection, normal);
		// Add specular compoment
		//float RdotE = max(dot(R, normalize(-position)), 0.0);
		//Color += vec4(LightAtt * LightColor * specularColor.rgb * pow(RdotE, specularColor.a),16.0);
	}
	
	// Add diffuse color 
	Color *= vec4(diffuseColor,1.0);
	Color *= ShadowFactor;
	
	// ============ Compute Indirect Illumination
	vec4 IndirectTerm = vec4(0.0);
	float rayonMax = 0.2;
	vec2 fres = vec2(20,20);
	vec3 random = vec3(rand(shadowCoordinateWdivide.st*fres.xy));
	for(int i = 0; i < 84; i++) // FIXME
		{
			//Generate a grid coordinate from a single looping variable.
			float x = ((i%9)/9.0)-0.5;
			float y = (i/81.0)-0.5;

			x += random.x;
			y += random.y;

			float vX = texture(NoiseBuffer,vec2(x,y)).r;
			float vY = texture(NoiseBuffer,vec2(x,y)).b;

			// Compute RSM Coords
			vec2 CoordTexRSM = vec2(shadowCoordinateWdivide.s+rayonMax*vX*sin(2*M_PI*vY),
									shadowCoordinateWdivide.t+rayonMax*vX*cos(2*M_PI*vY));
			CoordTexRSM = clamp(CoordTexRSM,0.0,1.0);
			// Get all informations form RSM
			vec3 NormalVPL = normalize(texture(NormalRSM, CoordTexRSM).xyz * 2.0 - 1.0);
			vec3 PositionVPL = texture(PositionRSM,CoordTexRSM).xyz + NormalVPL*0.0001;
			vec4 FluxVPL = texture(FluxRSM,CoordTexRSM);
			// Shadow Map RSM
			vec4 ShadowCoordRSM = biasMatrix*LightProjectionMatrix * LightViewMatrix * vec4(PositionVPL,1.0);
			vec4 shadowCoordinateWdivideRSM = ShadowCoordRSM / ShadowCoordRSM.w ;
			shadowCoordinateWdivideRSM.z -= 0.001;
			float ClosedLightDistanceRSM = texture(DepthRSM, shadowCoordinateWdivideRSM.st).r;
			float ShadowFactorRSM = ClosedLightDistanceRSM < shadowCoordinateWdivideRSM.z ? 0.0 : 1.0 ;
			// Add compute term
			vec3 VPLDirection = PositionVPL - position;
			float VPLDistance = length(VPLDirection);
			VPLDirection = normalize(VPLDirection);
			float factor = vY*vY / pow(VPLDistance,4);
			//ShadowFactorRSM*
			vec4 RadianceFlux = FluxVPL * max(0,dot(normal,VPLDirection));
			IndirectTerm += RadianceFlux * factor;
			//IndirectTerm += vec4(vX,vY,0.5,1.0) * ( 1.0 / 84.0 );
		}
	//Color += IndirectTerm;
	if(DebugMode)
	{
	   float distanceFromLight = LinearizeDepth(ClosedLightDistance);
       Color = vec4(clamp(IndirectTerm,0.0,1.0)*1.0);
	}
}
