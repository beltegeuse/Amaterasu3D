#version 130

//invariant gl_Position;
flat out float factor;

void main()
{
	//Transform the vertex (ModelViewProj matrix)
	factor = sign(gl_Vertex.x);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
