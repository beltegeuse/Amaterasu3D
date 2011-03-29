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

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

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
	// Get data
	vec3 Position = texture(PositionBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);

	// Get Grid Coordinates FIXME
	Position = (Position-LPVPosition) / LPVCellSize.xyz;

	vec2 TexCoordGrid = Map3DPosTo2D(Position);
	vec4 CoeffGrid = texture2D(Grid, TexCoordGrid);
	if(CoeffGrid == vec4(0,0,0,1))
		Color = CoeffGrid;
	else
		Color = vec4(1.0,0.0,0.0,1.0); // FIXME
}
