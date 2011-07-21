#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(points, max_vertices = 128)out; // TODO

// Precision qualifier
precision highp float;

// Out (Offset modifications)
out flat vec4 data;

void main()
{
	// Here put the ray casting algorithm
}
