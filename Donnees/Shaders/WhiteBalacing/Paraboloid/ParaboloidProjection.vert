// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec2 VertexTexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

// Smooth variables
smooth out vec2 outTexCoord;
smooth out float outClipDepth;
// For paraboloid projection
uniform float FarClipping;
uniform float NearClipping;

invariant gl_Position;

void main ()
{
    outTexCoord = VertexTexCoord;
    
	vec4 CamPosition = ViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0);
	//CamPosition /= CamPosition.w;
	
	float L = length(CamPosition.xyz);
	CamPosition = CamPosition / L;
	
	outClipDepth = CamPosition.z;
	
	CamPosition.z = CamPosition.z + 1;
	CamPosition.x = CamPosition.x / CamPosition.z;
	CamPosition.y = CamPosition.y / CamPosition.z;
	
	CamPosition.z = (L - NearClipping) / (FarClipping - NearClipping);
	CamPosition.w = 1;
	
	gl_Position = CamPosition;
}
