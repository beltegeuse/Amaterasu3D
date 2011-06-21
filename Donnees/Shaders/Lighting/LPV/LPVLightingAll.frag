#version 130

// Precision qualifier
precision highp float;

// Textures
// ************** LPV Grids
uniform sampler2D GridRed;
uniform sampler2D GridGreen; ///< Reprensent SH Grid
uniform sampler2D GridBlue;

uniform sampler2D NormalBuffer;
uniform sampler2D DiffuseBuffer;
uniform sampler2D SpecularBuffer;
uniform sampler2D ShadowBuffer;
uniform sampler2D DepthBuffer;

// Parametres
// NB_CASCADE is an define...
uniform vec3 LPVPosition[NB_CASCADE]; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform float LPVCellSize[NB_CASCADE]; // dim &
uniform int LPVNbCell;// number cell in one dim
uniform bool EnableTrilinearInterpolation;

// Parameters to compute position form Depth
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

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

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

// Include compute position from the depth
#include <GetPosition.shadercode>
// Include for SH coefficents
#include <LPVSH.shadercode>
// Include to compute the position
#include <LPVPosition.shadercode>
// Include for the trilinear filtering
#include <LPVTrilinear.shadercode>

float ComputeShadow(in vec3 position)
{
	// **** Compute coordinates
	//vec2 ShadowOffset = vec2(1.0/2048.0, 1.0/2048.0);
	vec2 ShadowOffset = vec2(2.0/2048.0, 2.0/2048.0);
	mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
						   0.0, 0.5, 0.0, 0.0,
						   0.0, 0.0, 0.5, 0.0,
						   0.5, 0.5, 0.5, 1.0);
	vec4 ShadowCoord = biasMatrix*LightProjectionMatrix * LightViewMatrix * vec4(position,1.0);
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;

	// ***  Check if we are inside the shadow map
	float inside = step(0.0,shadowCoordinateWdivide.s) * step(shadowCoordinateWdivide.s,1.0);
	inside *= step(0.0,shadowCoordinateWdivide.t) * step(shadowCoordinateWdivide.t,1.0);
	inside *= step(0.0,shadowCoordinateWdivide.z);


	// *** takes samples for PCF
	vec4 samples1;
    samples1.x = texture(ShadowBuffer, shadowCoordinateWdivide.st+ShadowOffset * vec2( 0.0, 1.0)).r;
    samples1.y = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2( 1.0, 0.0)).r;
    samples1.z = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2( 0.0,-1.0)).r;
    samples1.w = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2(-1.0, 1.0)).r;

    vec4 samples2;
    samples2.x = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2(-1.0, 2.0)).r;
    samples2.y = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2( 2.0, 1.0)).r;
    samples2.z = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2( 1.0,-2.0)).r;
    samples2.w = texture(ShadowBuffer,shadowCoordinateWdivide.st+ShadowOffset * vec2(-2.0,-1.0)).r;

    float shadow = dot(step(shadowCoordinateWdivide.z - 0.001,samples1),vec4(0.125,0.125,0.125,0.125))
			   + dot(step(shadowCoordinateWdivide.z - 0.001,samples2),vec4(0.125,0.125,0.125,0.125));
    shadow = mix(1.0,shadow,inside);
    shadow = shadow;
	if(any(lessThan(shadowCoordinateWdivide.xyz,vec3(0.0))) || any(greaterThan(shadowCoordinateWdivide.xyz,vec3(1.0))) || ShadowCoord.w < 0.0)
		return 0.0;

//	float shadow = step(shadowCoordinateWdivide.z - 0.001,texture(ShadowBuffer, shadowCoordinateWdivide.st).r)*1.0;
    return shadow;
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

	// Shadow Map
	float ShadowFactor = ComputeShadow(position);

	// Compute light attenation
	//float SpotAtt = pow(SpotDot, 12.0); //TODO: uniform ???
	//float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;
	float LightAtt = LightIntensity / (1.0+LightDistance*LightDistance);
	
	// Initialise Black color
	Color = vec4(0.0);

	// Verify the light and the normal is in the same direction
	float NdotL = max(dot(normal, LightDirection), 0.0);
	if (NdotL > 0.0) {
		// Add diffuse compoment
		Color += LightAtt*vec4((LightColor.rgb),1.0)*NdotL; //NdotL
		// Compute reflect vector
		//vec3 R = reflect(-LightDirection, normal);
		// Add specular compoment
		//float RdotE = max(dot(R, normalize(-position)), 0.0);
		//Color += clamp(vec4(LightAtt * LightColor.rgb * specularColor.rgb * pow(RdotE, specularColor.a),16.0),0.0,1.0);
	}


	// Add diffuse color
	Color *= vec4(diffuseColor,1.0);
	Color *= ShadowFactor;

	Color += vec4(diffuseColor*vec3(ComputeIndirectLighting(position, normal)),1.0);
	Color *= 0.5;
	//Color += vec4(diffuseColor,1.0)*vec4(vec3(0.05),1.0);
}
