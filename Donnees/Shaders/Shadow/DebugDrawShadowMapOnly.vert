// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;

// Uniform
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform mat4 LightViewMatrix;
uniform mat4 LightProjectionMatrix;

// Smooth variables
smooth out vec4 outShadowCoord;

invariant gl_Position;

void main ()
{
	mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
						   0.0, 0.5, 0.0, 0.0,
						   0.0, 0.0, 0.5, 0.0,
						   0.5, 0.5, 0.5, 1.0);
    outShadowCoord = biasMatrix * (LightProjectionMatrix * LightViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4 (VertexPosition, 1.0);
}
