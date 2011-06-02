#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D LDRBuffer;

// Shader input
smooth in vec2 outTexCoord;

// Shader output
out vec4 ColorBuffer;

#include <ColorimetryHelper.shadercode>

void main()
{
	vec3 WhiteRef = textureLod(LDRBuffer, outTexCoord, 10).rgb;
	vec3 Color = textureLod(LDRBuffer, outTexCoord, 0).rgb;

	ColorBuffer = vec4(XYZtoRGB(ChromaticAdaptationCAT02(RGBtoXYZ(Color*255), RGBtoXYZ(WhiteRef*255))) / 255.0 ,1.0);
	//ColorBuffer = vec4(XYZtoRGB(RGBtoXYZ(Color)),1.0);
}
