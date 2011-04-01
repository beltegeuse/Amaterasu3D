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
uniform vec3 positionCamera;
uniform vec4 LightColor;
uniform float LightRaduis;
uniform float LightIntensity;
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
	
	if (DebugMode == false)
	{
		if(LightDistance > LightRaduis)
			discard;
	}	
	
	// Compute light attenation
	float LightAtt = clamp(1.0 - LightDistance/LightRaduis, 0.0, 1.0) * LightIntensity;
	
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
		Color += vec4(LightAtt * LightColor.rgb * specularColor.rgb * pow(RdotE, specularColor.a),0.0);
	}
	
	// Add diffuse color 
	Color *= vec4(diffuseColor,1.0);
	
	if(DebugMode)
	{
		Color = vec4(LightAtt);
		if (LightDistance > LightRaduis - 0.02 && LightDistance < LightRaduis + 0.02)
			Color = vec4(1.0, 0.0, 0.0, 0.0);
	}
}
