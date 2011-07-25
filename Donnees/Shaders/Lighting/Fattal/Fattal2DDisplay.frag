#version 130

// Precision qualifier
precision highp float;

// Texture
uniform sampler2D Databuffer;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

void main()
{	
	Color = vec4(vec3(texture(Databuffer, outTexCoord).a),1.0);
}
