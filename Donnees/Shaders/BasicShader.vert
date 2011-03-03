// VERTEX SHADER
#version 130

// Attributs
in vec3 VertexPosition;

// Uniform
uniform mat4 ModelViewMatrix, ModelViewProjectionMatrix;

invariant gl_Position;

void main ()
{
	vec3 Position = vec3 (ModelViewMatrix * vec4 (VertexPosition, 1.0));
	gl_Position = ModelViewProjectionMatrix * vec4 (VertexPosition, 1.0);
}
