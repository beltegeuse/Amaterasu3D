// VERTEX SHADER
#version 130

uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

// Sortie shader
smooth out vec2 outTexCoord1;
smooth out vec2 outTexCoord2;
smooth out float intensity_sq;
smooth out float intensity;

invariant gl_Position;

uniform float time;

void main()
{
   gl_Position = ProjectionMatrix * ModelMatrix * ViewMatrix * gl_Vertex;

   float myTime = time /10.0;

   outTexCoord1 = vec2(gl_Vertex.x / 4.0 + myTime,       gl_Vertex.z / 4.0);
   outTexCoord2 = vec2(gl_Vertex.x      + myTime / 2.0, gl_Vertex.z);

// Get the vertex height and scale it to 0 - 1 range.
   intensity    = gl_Vertex.y * 0.5; // / bbox.y;
   intensity    = exp(clamp(intensity, 0.0, 1.0)) / 1.8;
   intensity_sq = intensity * intensity;
}
