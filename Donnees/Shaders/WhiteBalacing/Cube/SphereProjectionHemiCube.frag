#version 130

// Precision qualifier
precision highp float;

// Texture unit
uniform sampler2D ZTex;
uniform sampler2D YPlusTex;
uniform sampler2D YMoinsTex;
uniform sampler2D XPlusTex;
uniform sampler2D XMoinsTex;

// Shader input
smooth in vec2 outTexCoord;

// Shader output
out vec4 ColorBuffer;

#define V 0.707106781
#define Vinv 1.41421356
#define PI 3.1415926535

void main()
{
    vec2 NormTexCoord = 2*outTexCoord - 1;
    if(length(NormTexCoord) > 1)
    	ColorBuffer = vec4(0.0);
    else
    {

    	vec3 N;
    	N.xy = -NormTexCoord.xy;
    	N.z = sqrt(1 - NormTexCoord.x*NormTexCoord.x - NormTexCoord.y*NormTexCoord.y);
    	N = normalize(N);

    	vec3 AbsN = abs(N);
    	vec3 SignN = sign(N);

    	ColorBuffer = vec4(0.5*(vec3(N)+1), 1.0);
    	if(AbsN.z > AbsN.x && AbsN.z > AbsN.y)
    		ColorBuffer = texture(ZTex, (N.xy/N.z + vec2(1))*0.5);
    	else if(AbsN.x > AbsN.y)
    	{
    		vec2 Coord = N.yz/N.x;
    		if(SignN.x == -1)
    			ColorBuffer = texture(XPlusTex, vec2(abs(Coord.y), 1-(Coord.x+1)*0.5));
    		else
    			ColorBuffer = texture(XMoinsTex, vec2(1-Coord.y, (Coord.x+1)*0.5));
    	}
    	else
    	{
    		vec2 Coord = N.xz/N.y;
    		if(SignN.y == -1)
				ColorBuffer = texture(YPlusTex,vec2(1-(Coord.x+1)*0.5, abs(Coord.y)));
			else
				ColorBuffer = texture(YMoinsTex,vec2((Coord.x+1)*0.5, 1-Coord.y));
    	}
    }
}
