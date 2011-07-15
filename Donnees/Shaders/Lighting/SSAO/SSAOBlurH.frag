#version 130

// Precision qualifier
precision highp float;

// In value
smooth in vec2 outTexCoord;

// Texture to blur
uniform sampler2D TextureBuffer;

// Output buffer
out vec4 Result;

{% set Width = 800 %}
{% set InvCoordH = 1.0/Width %}
{% set NumCoeff = 9 %}
{% set Coeff = [0.0677841, 0.0954044, 0.121786, 0.140999, 0.148054, 0.140999, 0.121786, 0.0954044, 0.0677841] %}
void main()
{
	vec4 Sum = vec4(0.0);
	{% for i in range(NumCoeff) %}
		{% set iR = (i-4)*InvCoordH %}
		{% set coeffI = Coeff[i] %}
		Sum += {{coeffI}}*texture(TextureBuffer,outTexCoord+vec2({{iR}},0));
	{% endfor %}
	Result = Sum;
}
