// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexTangent;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 VertexColor;

// Uniform States
uniform int UseBumpMapping;
uniform int UseDiffuseTex;
uniform int UseSpecularTex;

// Uniform matrix
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

// Smooth variables
smooth out vec2 outTexCoord;
smooth out mat3 outtnbMatrix;
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
	    vec3 normal = normalize(NormalMatrix * VertexNormal);
		vec3 tangent = normalize(NormalMatrix * VertexTangent.xyz);
		vec3 binormal = normalize(-1.0 * cross(VertexNormal, VertexTangent)); // remove w for VertexTangant
		outtnbMatrix =  mat3(tangent.x,binormal.x, normal.x,
		                    tangent.y, binormal.y, normal.y,
		                    tangent.z,binormal.z,normal.z);
	}
	else
	{
	    outNormal = normalize(NormalMatrix * VertexNormal);
	}
    
    // Diffuse management
    if(UseDiffuseTex == 0)
    {
        outColor = VertexColor;
    }
    
    
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (VertexPosition, 1.0);
}
