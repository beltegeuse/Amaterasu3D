#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D TextureDiffuse;

// Shader input
smooth in vec2 outTexCoord;
smooth in float outClipDepth;

// Shader output
out vec4 ColorBuffer;

void main()
{
	if(outClipDepth < 0.0)
		discard;

    // Diffuse buffer
	ColorBuffer = texture(TextureDiffuse, outTexCoord.st);
}
