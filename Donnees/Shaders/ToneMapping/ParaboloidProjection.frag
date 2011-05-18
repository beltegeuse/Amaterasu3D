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

// Material
uniform vec4 MaterialDiffuseColor;
uniform vec4 MaterialSpecularColor;

// Shader input
smooth in vec2 outTexCoord;
smooth in mat3 outtbnMatrix;
smooth in vec3 outColor;
smooth in vec3 outNormal;

// Shader output
out vec4 Buffer;

void main()
{
    // Diffuse buffer
	Buffer = MaterialDiffuseColor;
    if(UseDiffuseTex == 1)
    {
		Buffer *= texture(TextureDiffuse, outTexCoord.st);
	}
	else if(outColor != vec3(0.0)) // FIXME: Impossible couleur noire ???
	{
	    Buffer *= vec4(outColor,1.0);
	}
}
