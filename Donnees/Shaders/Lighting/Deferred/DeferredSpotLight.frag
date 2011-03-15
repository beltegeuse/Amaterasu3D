#version 130

// Precision qualifier
precision highp float;

//Les differents buffers
uniform sampler2D DiffuseBuffer;
uniform sampler2D SpecularBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D PositionBuffer;

// light caracteristics
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform vec3 LightSpotDirection;
uniform float LightRaduis;
uniform float LightIntensity;
uniform float LightCutOff; // cos value

// To enable / disable the debug mode
uniform bool DebugMode;

// Entree
smooth in vec2 outTexCoord;

// Output buffers
out vec4 Color;

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
	
	// Compute light attenation
    float SpotAtt = pow(SpotDot, 12.0); //TODO: uniform ???
	float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity * SpotAtt;
	
	// Initialise Black color
	Color = vec4(0.0);
	
	// Verify the light and the normal is in the same direction
	float NdotL = max(dot(normal, LightDirection), 0.0);
	if (NdotL > 0.0) {
		// Add diffuse compoment
		Color += vec4(LightAtt * (LightColor * NdotL),1.0);
		// Compute reflect vector
		vec3 R = reflect(-LightDirection, normal);
		// Add specular compoment
		float RdotE = max(dot(R, normalize(-position)), 0.0);
		Color += vec4(LightAtt * LightColor * specularColor.rgb * pow(RdotE, specularColor.a),16.0);
	}
	
	// Add diffuse color 
	Color *= vec4(diffuseColor,1.0);
	
	if(DebugMode)
	{
		Color = vec4(LightAtt);
		if (LightDistance > LightRaduis - 0.02 && LightDistance < LightRaduis + 0.02)
			Color = vec4(1.0, 0.0, 0.0, 0.0);
		if ((SpotDot > LightCutOff - 0.001) &&  (SpotDot < LightCutOff + 0.001))
			Color = vec4(0.0, 1.0, 0.0, 0.0);
	}
}