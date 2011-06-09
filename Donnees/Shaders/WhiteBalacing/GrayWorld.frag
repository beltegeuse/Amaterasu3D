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
	vec3 Color = textureLod(LDRBuffer, outTexCoord, 0).rgb;
	vec3 WhiteRef = textureLod(LDRBuffer, outTexCoord, 10).rgb*255;
	//vec3 WhiteRef = vec3(94,80,69);
	//vec3 WhiteRef = vec3(94.5361,100.0000,88.6811);
	vec3 WhiteRefXYZ = RGBtoXYZ(WhiteRef); //vec3(76.9499,81.3973,72.1840);

	vec3 WhiteRefxyY = XYZtoxyY(WhiteRefXYZ);
	WhiteRefxyY.z = 100.0;

	WhiteRef = xyYtoXYZ(WhiteRefxyY);

	ColorBuffer = vec4(ChromaticAdaptationCAT02(Color, WhiteRef),1.0);
	
	
	//ColorBuffer = vec4(XYZtoRGB(xyYtoXYZ(XYZtoxyY(RGBtoXYZ(Color)))),1.0);
	//ColorBuffer = vec4(Color*transpose(MCAT02)*transpose(MCAT02Inv),1.0);
	//ColorBuffer = vec4(XYZtoRGB(xyYtoXYZ(WhiteRefxyY)) / 255,1.0);
	//ColorBuffer = vec4(RGBtoXYZ(Color),1.0);
//	ColorBuffer = vec4(WhiteRef / 255.0,1.0);
//	WhiteRefxyY = XYZtoxyY(RGBtoXYZ(Color*255.0));
//	WhiteRefxyY.z = 100.0;
//	ColorBuffer = vec4(XYZtoRGB(xyYtoXYZ(WhiteRefxyY)) / 255.0,1.0);
}
