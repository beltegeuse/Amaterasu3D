// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexTangent;
in vec3 VertexBiTangent;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 VertexColor;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;

// Uniform States
uniform int UseBumpMapping;
uniform int UseDiffuseTex;
uniform int UseSpecularTex;

// Smooth variables
smooth out vec3 outPosition;
smooth out vec2 outTexCoord;
smooth out mat3 outtbnMatrix;
smooth out vec3 outColor;
smooth out vec3 outNormal;

invariant gl_Position;

void main ()
{

	mat3 NormalMatrix3 = mat3(NormalMatrix);

    // Texture coordinates
	if(UseBumpMapping == 1 || UseDiffuseTex == 1 || UseSpecularTex == 1)
	{
		outTexCoord = VertexTexCoord;
	}
	
	// Normal management
	if(UseBumpMapping == 1)
	{
	    vec3 normal = normalize(NormalMatrix3 * VertexNormal);
		vec3 tangent = normalize(NormalMatrix3 * VertexTangent);
		vec3 binormal = normalize(NormalMatrix3 * VertexBiTangent);
		outtbnMatrix = mat3(tangent, binormal, normal);
	}
	else
	{
	    outNormal = normalize(NormalMatrix3 * VertexNormal);
	}
    
    // Diffuse management
    if(UseDiffuseTex == 0)
    {
        outColor = VertexColor;
    }
    
    outPosition = (ModelMatrix * vec4 (VertexPosition, 1.0)).xyz;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0);
}
