#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D Buffer;
uniform vec2 TexSize;

// Shader input
smooth in vec2 outTexCoord;

// Shader output
out vec4 ColorBuffer;

void main()
{
	ivec2 c = ivec2(floor(outTexCoord*TexSize));
	ColorBuffer = 0.25*texelFetchOffset(Buffer, c, 0, ivec2(0,0)) + 0.25*texelFetchOffset(Buffer, c, 0, ivec2(1,0))  + 0.25*texelFetchOffset(Buffer, c, 0, ivec2(0,1)) + 0.25*texelFetchOffset(Buffer, c, 0, ivec2(0,1));
}
