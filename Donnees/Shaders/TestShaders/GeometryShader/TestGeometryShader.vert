#version 130

invariant gl_Position;

void main()
{
	//Transform the vertex (ModelViewProj matrix)
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
