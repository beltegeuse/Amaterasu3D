#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FontTexture;

// Font Caracteristic
// -- Global font caracteristic
uniform vec4 FontColor;
// -- For choose the effect
uniform int FontEffectType;
uniform vec4 FontEffectColor;
//  ==== OutLine
const float OUTLINE_MIN_0 = 0.4;
const float OUTLINE_MAX_0 = 0.5;
//  ==== Soft Edge
const float SOFTEDGE_MIN_0 = 0.4;
const float SOFTEDGE_MAX_0 = 0.5;
//  ==== OuterGlow
// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

void main()
{	
	vec4 baseColor = FontColor;
	baseColor.a = texture2D(FontTexture, outTexCoord.xy).a;
	float distanceFactor = baseColor.a;
	
	float width = fwidth(outTexCoord.x) * 10.0; // 25 is an arbitrary scale factor to get an appropriate width for antialiasing
	baseColor.a = smoothstep(0.5-width, 0.5+width, baseColor.a);
	
	if(FontEffectType == 0)
	{
		//////////////////////////////////
		// For outline
		//////////////////////////////////
		// Outline constants
		float OUTLINE_MIN_1 = OUTLINE_MIN_0 + width * 2;
		float OUTLINE_MAX_1 = OUTLINE_MAX_0 + width * 2;
		
		// Outline calculation
		if (distanceFactor > OUTLINE_MIN_0 && distanceFactor < OUTLINE_MAX_1)
		{
			float outlineAlpha;
			if (distanceFactor < OUTLINE_MIN_1)
				outlineAlpha = smoothstep(OUTLINE_MIN_0, OUTLINE_MIN_1, distanceFactor);
			else
				outlineAlpha = smoothstep(OUTLINE_MAX_1, OUTLINE_MAX_0, distanceFactor);
				
			baseColor = mix(baseColor, FontEffectColor, outlineAlpha);
		}
	}
	
	if(FontEffectType == 1)
	{
		baseColor.a *= smoothstep(SOFTEDGE_MIN_0, SOFTEDGE_MAX_0, distanceFactor);
	}
	else
	{
		//baseColor.a = int(distanceFactor >= 0.5);
	}
	
	if(FontEffectType == 2)
	{
		// Shadow / glow constants
		const vec2 GLOW_UV_OFFSET = vec2(-0.004, -0.004);

		// Shadow / glow calculation
		float glowDistance = texture2D(FontTexture, outTexCoord.xy + GLOW_UV_OFFSET).a;
		float glowFactor = smoothstep(0.3, 0.5, glowDistance);
		
		baseColor = mix(vec4(FontEffectColor.rgb, glowFactor), baseColor, baseColor.a);
	}
	
	Color = baseColor;
}