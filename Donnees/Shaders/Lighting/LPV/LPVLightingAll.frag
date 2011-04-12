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
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim
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

#include <GetPosition.shadercode>

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

#include <LPVSH.shadercode>
#include <LPVPosition.shadercode>

// Trilinear interpolation
// \position : It's grid position
//TODO: Add border support
vec4 TrilinearInterpolation(sampler2D s, vec3 Position, vec3 Normal)
{
	vec3 IndexedPosition = floor(Position);
	vec3 Offset = Position - IndexedPosition;

	vec4 v1i1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition));
	vec4 v2i1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,0,1)));
	vec4 i1 = v1i1*(1-Offset.z)+v2i1*Offset.z;

	vec4 v1i2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,1,0)));
	vec4 v2i2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,1,1)));
	vec4 i2 = v1i2*(1-Offset.z)+v2i2*Offset.z;

	vec4 v1j1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,0,0)));
	vec4 v2j1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,0,1)));
	vec4 j1 = v1j1*(1-Offset.z)+v2j1*Offset.z;

	vec4 v1j2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,1,0)));
	vec4 v2j2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,1,1)));
	vec4 j2 = v1j2*(1-Offset.z)+v2j2*Offset.z;

	vec4 w1 = i1*(1 - Offset.y) + i2 * Offset.y;
	vec4 w2 = j1*(1 - Offset.y) + j2 * Offset.y;

	return w1*(1 - Offset.x) + w2 * Offset.x;
}

vec4 TrilinearInterpolationWorld(sampler2D s, vec3 Position, vec3 Normal)
{
	vec3 PositionGrid = ((Position-LPVPosition) / LPVCellSize.xyz) - 0.5;
	return TrilinearInterpolation(s,PositionGrid, Normal);
}

vec4 sample3D(in sampler2D s, in vec3 coords){
  float CellCount = (LPVCellSize.w-1.0);
  vec3 c = clamp(coords,0.0,1.0) * CellCount;

  vec3 c1 = vec3(c.x,c.y,floor(c.z));
  vec3 c2 = clamp(c1+vec3(0.0,0.0,1.0),0.0,CellCount);

	float row = floor(c1.z / LPVSize.z);
	float col = c1.z - (row*LPVSize.z);
	vec2 spos1 = vec2(col * LPVCellSize.w + c1.x,row * LPVCellSize.w + c1.y) / LPVSize.xy;

	row = floor(c2.z / LPVSize.z);
	col = c2.z - (row*LPVSize.z);
	vec2 spos2 = vec2(col * LPVCellSize.w + c2.x,row * LPVCellSize.w + c2.y) / LPVSize.xy;

	vec4 s1 = texture2D(s,spos1);
	vec4 s2 = texture2D(s,spos2);

	return mix(s1,s2,c.z-c1.z);
}

float ComputeShadow(in vec3 position)
{
	// **** Compute coordinates
	vec2 ShadowOffset = vec2(1.0/2048.0, 1.0/2048.0);
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

    float shadow = dot(step(shadowCoordinateWdivide.z - 0.0001,samples1),vec4(0.125,0.125,0.125,0.125))
			   + dot(step(shadowCoordinateWdivide.z - 0.0001,samples2),vec4(0.125,0.125,0.125,0.125));
    shadow = mix(1.0,shadow,inside);
    shadow = shadow;

    return shadow;
}

vec4 ComputeIndirectLighting(in vec3 Position, in vec3 Normal)
{
	// Get Grid Coordinates
	//Position = floor((Position.xyz - LPVPosition) / LPVCellSize.xyz);

	// Get texture coordinates
	//vec2 TexCoordGrid = Convert3DTo2DTexcoord(Position);
	vec4 CoeffGridRed = TrilinearInterpolationWorld(GridRed,Position, Normal);; ///< And get coeff value
	vec4 CoeffGridGreen = TrilinearInterpolationWorld(GridGreen,Position, Normal);;
	vec4 CoeffGridBlue = TrilinearInterpolationWorld(GridBlue,Position, Normal);;

	vec4 SHEv = SH_evaluate(-Normal);
	return  vec4(clamp(4.0*dot(CoeffGridRed,SHEv),0,1),
					clamp(4.0*dot(CoeffGridGreen,SHEv),0,1),
					clamp(4.0*dot(CoeffGridBlue,SHEv),0,1),1.0);
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
	float SpotAtt = pow(SpotDot, 12.0); //TODO: uniform ???
	float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;

	// Initialise Black color
	Color = vec4(0.0);

	// Verify the light and the normal is in the same direction
	float NdotL = max(dot(normal, LightDirection), 0.0);
	if (NdotL > 0.0) {
		// Add diffuse compoment
		Color += vec4((LightColor.rgb),1.0); //NdotL
		// Compute reflect vector
		vec3 R = reflect(-LightDirection, normal);
		// Add specular compoment
		float RdotE = max(dot(R, normalize(-position)), 0.0);
		Color += clamp(vec4(LightAtt * LightColor.rgb * specularColor.rgb * pow(RdotE, specularColor.a),16.0),0.0,1.0);
	}

	// Add diffuse color
	Color *= vec4(diffuseColor,1.0);
	Color *= ShadowFactor;

	Color += vec4(diffuseColor,1.0)*ComputeIndirectLighting(position, normal);
	//Color.a = 1.0;
}
