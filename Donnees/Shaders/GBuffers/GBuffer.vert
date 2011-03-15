// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexTangent;
in vec3 VertexBiTangent;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 VertexColor;

// Uniform States
uniform int UseBumpMapping;
uniform int UseDiffuseTex;
uniform int UseSpecularTex;

// Smooth variables
smooth out vec4 outPosition;
smooth out vec2 outTexCoord;
smooth out mat3 outtbnMatrix;
smooth out vec3 outColor;
smooth out vec3 outNormal;

invariant gl_Position;

void main ()
{
    // Texture coordinates
	if(UseBumpMapping == 1 || UseDiffuseTex == 1 || UseSpecularTex == 1)
	{
		outTexCoord = VertexTexCoord;
	}
	
	// Normal management
	if(UseBumpMapping == 1)
	{
	    //vec3 normal = normalize(ModelViewMatrix * vec4(VertexNormal,0.0)).xyz;
		//vec3 tangent = normalize(ModelViewMatrix * vec4(VertexTangent,0.0)).xyz;
		//vec3 binormal = normalize(ModelViewMatrix * vec4(VertexBiTangent,0.0)).xyz;
	    vec3 normal = normalize(gl_NormalMatrix * VertexNormal);
		vec3 tangent = normalize(gl_NormalMatrix * VertexTangent);
		vec3 binormal = normalize(gl_NormalMatrix * VertexBiTangent);
		outtbnMatrix = mat3(tangent, binormal, normal);
	}
	else
	{
	    outNormal = normalize(gl_NormalMatrix * VertexNormal);
	}
    
    // Diffuse management
    if(UseDiffuseTex == 0)
    {
        outColor = VertexColor;
    }
    
    outPosition = gl_ModelViewMatrix * vec4 (VertexPosition, 1.0);
	gl_Position = gl_ProjectionMatrix * outPosition;
}
