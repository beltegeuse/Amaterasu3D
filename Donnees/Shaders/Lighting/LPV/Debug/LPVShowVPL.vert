#version 130

// Precision qualifier
precision highp float;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

// Textures
uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;

// Parametres
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

smooth out vec2 outTexCoord;

invariant gl_Position;

void main()
{	
	outTexCoord.x = gl_Vertex.x;
	outTexCoord.y = gl_Vertex.y;

	// Get data
	vec3 Position = texture(PositionBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);

	Position += Normal; // Offset

	gl_Position = ProjectionMatrix*ViewMatrix*vec4(Position,1.0);
}
