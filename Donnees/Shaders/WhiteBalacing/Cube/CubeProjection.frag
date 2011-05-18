#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D TextureDiffuse;

// Shader input
smooth in vec2 outTexCoord;

// Shader output
out vec4 ColorBuffer;

void main()
{
    // Diffuse buffer
	ColorBuffer = texture(TextureDiffuse, outTexCoord.st);
}
