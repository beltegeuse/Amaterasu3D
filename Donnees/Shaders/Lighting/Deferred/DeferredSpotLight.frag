#version 130

// Precision qualifier
precision highp float;

//Les differents buffers
uniform sampler2D DiffuseBuffer;
uniform sampler2D SpecularBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D PositionBuffer;
uniform sampler2D ShadowBuffer;

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

// Entree
smooth in vec2 outTexCoord;

// Output buffers
out vec4 Color;

float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = LightRaduis; // camera z far
  float z = texture2D(ShadowBuffer, uv).x;
  return ((2.0 * n) / (f + n - z * (f - n))) * f;
}

void main()
{	
	// Inspired by : http://encelo.netsons.org/blog/tag/glsl/
	
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
	
	float ClosedLightDistance = texture(ShadowBuffer, shadowCoordinateWdivide.st).r;
	float ShadowFactor = ClosedLightDistance < shadowCoordinateWdivide.z ? 0.2 : 1.0 ;
	
	
	// Compute light attenation
    float SpotAtt = pow(SpotDot, 12.0); //TODO: uniform ???
	float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;
	
	// Initialise Black color
	Color = vec4(0.0);
	
	// Verify the light and the normal is in the same direction
	float NdotL = max(dot(normal, LightDirection), 0.0);
	if (NdotL > 0.0) {
		// Add diffuse compoment
		Color += vec4(LightAtt * (LightColor.rgb * NdotL),1.0);
		// Compute reflect vector
		vec3 R = reflect(-LightDirection, normal);
		// Add specular compoment
		float RdotE = max(dot(R, normalize(-position)), 0.0);
		Color += vec4(LightAtt * LightColor.rgb * specularColor.rgb * pow(RdotE, specularColor.a),16.0);
	}
	
	// Add diffuse color 
	Color *= vec4(diffuseColor,1.0);
	Color *= ShadowFactor;
	
	if(DebugMode)
	{
		Color = vec4(LightAtt);
		if (LightDistance > LightRaduis - 0.02 && LightDistance < LightRaduis + 0.02)
			Color = vec4(1.0, 0.0, 0.0, 0.0);
		if ((SpotDot > LightCutOff - 0.001) &&  (SpotDot < LightCutOff + 0.001))
			Color = vec4(0.0, 1.0, 0.0, 0.0);
		
		// Other Debug
		//Color = vec4(position.z, 0.0,0.0,1.0);
		//Color = vec4(vec3(LinearizeDepth(shadowCoordinateWdivide.st)),1.0);
	}
}
