#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D TextureDiffuse;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

float LinearizeDepth(float zoverw){
	float n = 1.0; // camera z near
	float f = 1000.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{	
    //FIXME: Put in uniform
	float z = min(1.0,LinearizeDepth(texture(TextureDiffuse, outTexCoord).r));
	Color = vec4(z,z,z,1.0);
	//Color = texture(TextureDiffuse, outTexCoord);
}