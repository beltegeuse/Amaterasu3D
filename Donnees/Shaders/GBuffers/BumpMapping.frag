#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;

// Shader input
smooth in vec2 outTexCoord;
smooth in mat3 outtnbMatrix;
smooth in vec4 outPosition; //TODO: Can optimize

// Shader output
out vec4 Position;
out vec4 Normal;
out vec4 Diffuse;

void main()
{	
	Position = vec4(outPosition.xyz, 0.0);
	Diffuse = texture2D(TextureDiffuse, outTexCoord.st);
	
	// Compute normals
	vec4 normalMap = texture2D(TextureNormal, outTexCoord.st);
	vec3 n = normalize(2.0 * normalMap.rgb - 1.0);
	vec3 normalInterpoled = n * outtnbMatrix;
	Normal = vec4(normalize(normalInterpoled),1.0);
}