#version 130

// Precision qualifier
precision highp float;

// Uniform data
uniform vec4 SpecularValue;

// Uniform States
uniform int UseBumpMapping;
uniform int UseDiffuseTex;
uniform int UseSpecularTex;

// Texture unit
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureNormal;
uniform sampler2D TextureSpecular;

// Shader input
smooth in vec2 outTexCoord;
smooth in mat3 outtbnMatrix;
smooth in vec3 outColor;
smooth in vec3 outNormal;

// Shader output
out vec4 Diffuse;
out vec4 Normal;
out vec4 Specular;
void main()
{
    // Diffuse buffer
    if(UseDiffuseTex == 1)
    {
		Diffuse = texture(TextureDiffuse, outTexCoord.st);
	}
	else
	{
	    Diffuse = vec4(outColor,1.0);
	}
	
	// Normals buffer
	if(UseBumpMapping == 1)
	{
		vec4 normalMap = texture(TextureNormal, outTexCoord.st);
		vec3 n = normalize(2.0 * normalMap.rgb - 1.0); // decode the Bump texture
		vec3 normalInterpoled = n * outtbnMatrix;
		Normal = vec4(normalize(normalInterpoled),1.0);
	}
	else
	{
	    Normal = vec4(normalize(outNormal), 1.0);
	}
	
	// Specular buffer
	if(UseSpecularTex == 1)
	{
	    // Warning use the end element of SpecularValue ton know the shininess
		Specular = vec4(texture(TextureSpecular, outTexCoord.st).rgb, SpecularValue.w);
	}
	else
	{
	    Specular  = vec4(1.0);
	}
}