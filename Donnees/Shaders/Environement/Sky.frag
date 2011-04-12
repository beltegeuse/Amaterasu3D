#version 130

// Precision qualifier
precision highp float;

//Uniform
uniform sampler2D CloudBuffer;

// Entree
smooth in vec2 outTexCoord1;
smooth in vec2 outTexCoord2;
smooth in float intensity_sq;
smooth in float intensity;

// Output buffers
out vec4 Color;

uniform vec4 horizon;

void main()
{
   vec4 clouds_0 = texture(CloudBuffer, outTexCoord1.xy);
   vec4 clouds_1 = texture(CloudBuffer, outTexCoord2.xy);

   vec4 clouds = (clouds_0 + clouds_1) * intensity_sq;

   vec4 cloud_color = vec4((1.0 - intensity) * horizon.x, (1.0 - intensity) * horizon.y, intensity * horizon.z, 0.0);

   Color = cloud_color * (1.0 - clouds.x) + clouds;
}
