#version 130

// Precision qualifier
precision highp float;
precision highp int;

//Les differents buffers
uniform sampler2D DiffuseBuffer;
uniform sampler2D SpecularBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;
uniform sampler2D DepthRSM;
uniform sampler2D NormalRSM;
uniform sampler2D FluxRSM;
uniform sampler2D NoiseBuffer;

// light caracteristics
uniform vec3 LightPosition;
uniform vec4 LightColor;
uniform vec3 LightSpotDirection;
uniform float LightRaduis;
uniform float LightIntensity;
uniform float LightCutOff; // cos value

// Shadow map
uniform mat4 LightViewMatrix;
uniform mat4 LightProjectionMatrix;

// To enable / disable the debug mode
uniform bool DebugMode;

// To Compute Position
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;
#include <GetPosition.shadercode>

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

void main()
{		
	// Get all data
	vec3 diffuseColor = texture(DiffuseBuffer, outTexCoord).xyz;
	vec4 specularColor = texture(SpecularBuffer, outTexCoord);
	vec3 normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	vec3 position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	
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
		Color += vec4(LightAtt * (LightColor.rgb * NdotL),1.0);
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
	for(int j = 0; j < 10.0; j++)
	for(int i = 0; i < 10.0; i++) // FIXME
		{
			// Get and compute random access
			vec2 coordOffset = (vec2(j/20.0,i/20.0)+20.0*shadowCoordinateWdivide.st);
			//vec2 coordOffset = (vec2(j,i))*vec2(1/512.0,1/512.0);
			vec2 CoordTexRSM = texture(NoiseBuffer,coordOffset).rg+shadowCoordinateWdivide.st;
			float factorNorm = texture(NoiseBuffer,coordOffset).b;
			//float factorNorm = 1.0;
			//vec2 CoordTexRSM = vec2((j-10.0),(i-10))*vec2(1/800.0,1/600.0)*12.0+shadowCoordinateWdivide.st;

			// Get all informations form RSM
			vec3 NormalVPL = normalize(texture(NormalRSM, CoordTexRSM).xyz * 2.0 - 1.0);
			vec3 PositionVPL = texture(PositionRSM,CoordTexRSM).xyz - NormalVPL*0.1;
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
			float factor = factorNorm / ((1.0+pow(VPLDistance,4))*2*M_PI);

			// ShadowFactorRSM *
			vec4 RadianceFlux =  ShadowFactorRSM * FluxVPL * max(0,dot(normal,VPLDirection)) * max(0,dot(NormalVPL,-VPLDirection));
			IndirectTerm += RadianceFlux * factor;
			//IndirectTerm += vec4(CoordTexRSM,0.5,1.0) * ( 1.0 / 400.0 );
		}
	Color += IndirectTerm;
	if(DebugMode)
	{
	   //float distanceFromLight = LinearizeDepth(ClosedLightDistance);
	   Color = vec4(clamp(IndirectTerm,0.0,1.0)*40.0);
		//vec4 Flux = texture(FluxRSM,shadowCoordinateWdivide.st);
		//Color = Flux;
	}
}
