// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec3 VertexTangent;
in vec3 VertexNormal;
in vec3 VertexTexCoord;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

// Smooth variables
smooth out vec2 outTexCoord;
smooth out mat3 outtnbMatrix;
smooth out vec4 outPosition;

invariant gl_Position;

void main ()
{
    // Out values
	outPosition = VertexColor * ModelViewMatrix * vec4 (VertexPosition, 1.0);
	outTexCoord = VertexTexCoord;
	
	// Compute outtnbMatrix
	vec3 tangent = normalize(NormalMatrix * VertexTangent.xyz);
	vec3 binormal = normalize(-VertexTangent.w * cross(VertexNormal, VertexTangent));
	outtnbMatrix = mat3(tangent.x, binormal.x, normal.x,
		tangent.y, binormal.y, normal.y,
		tangent.z, binormal.z, normal.z);
		
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4 (VertexPosition, 1.0);
}
