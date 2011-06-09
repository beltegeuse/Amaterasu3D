#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D Tex;

// Shader input
smooth in vec2 outTexCoord;

void main()
{
		vec4 textureColor = texture(Tex, outTexCoord);
		
		float fact = cos(radians((1-outTexCoord.t)*90));
		textureColor.rgb *= 0.75*fact*fact+0.25 ;

		gl_FragColor = textureColor; 
}
