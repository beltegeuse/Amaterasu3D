#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D Grid; ///< Reprensent SH Grid
uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;

// Parametres
uniform vec3 LPVPosition; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim

// Sortie shader
smooth out vec2 outTexCoord;
smooth out vec3 outNormal;
invariant gl_Position;

vec2 Map3DPosTo2D(in vec3 GridCoords)
{
	// Compute the indice of SubTexture
	float row = floor(GridCoords.z / LPVSize.z);
	float col = GridCoords.z - (row*LPVSize.z);
	// Compute Offset
	vec2 Coord = vec2(GridCoords.x+col*LPVCellSize.w,GridCoords.y+row*LPVCellSize.w);
	return Coord / LPVSize.xy;
}

void main()
{	
	outTexCoord.x = gl_Vertex.x;
	outTexCoord.y = gl_Vertex.y;

	// Get data
	vec3 Position = texture(PositionBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	Position += (Normal*LPVCellSize.xyz*0.5);

	Position = (Position-LPVPosition) / LPVCellSize.xyz;

	outNormal = Normal.xyz;
	gl_Position = vec4(Map3DPosTo2D(Position).xy*2.0-1.0,0.0,1.0);
}
