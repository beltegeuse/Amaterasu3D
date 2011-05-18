// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;
in vec2 VertexTexCoord;

uniform mat4 TransformMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

// Smooth variables
smooth out vec2 outTexCoord;

invariant gl_Position;

void main ()
{
    outTexCoord = VertexTexCoord;
    
	gl_Position = ProjectionMatrix * TransformMatrix * ViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0);
}
