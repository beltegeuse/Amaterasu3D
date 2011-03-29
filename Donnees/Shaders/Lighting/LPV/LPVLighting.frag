#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D Grid; ///< Reprensent SH Grid
uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;

// Parametres
uniform vec3 GridPosition;
uniform vec2 GridTextureSize;
float CellDemiSize = 5.0;
float CellSizeFactor = 1.0/10.0;
float CellPrecision = 1.0/32.0;
float LPVNumberCellX = 8.0;
float LPVNumberCellY = 4.0;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

vec3 ComputeGridCoordinates(vec3 Position)
{
	//TODO: Add boundary tests
	vec3 coords;
	coords.x = floor((Position.x-GridPosition.x)*CellSizeFactor);
	coords.y = floor((Position.y-GridPosition.y)*CellSizeFactor);
	coords.z = floor((Position.z-GridPosition.z)*CellSizeFactor);
	return coords;
}

vec2 Map3DPosTo2D(vec3 GridCoords)
{
	// Compute the indice of SubTexture
	float Xz = floor(GridCoords.z / LPVNumberCellY);
	float Yz = GridCoords.z - (Xz*LPVNumberCellY);
	// Compute Offset
	vec2 Coord = vec2(GridCoords.x+Xz*(1/CellPrecision),GridCoords.y+Yz*(1/CellPrecision));
	return Coord / GridTextureSize;
}

void main()
{	
	// Get data
	vec3 Position = texture(PositionBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);

	// Get Grid Coordinates FIXME
	vec3 GridCoords = ComputeGridCoordinates(Position+(Normal*CellDemiSize));
	vec2 TexCoordGrid = Map3DPosTo2D(GridCoords);
	vec4 CoeffGrid = texture2D(Grid, TexCoordGrid);
	if(CoeffGrid == vec4(0,0,0,1))
		Color = CoeffGrid;
	else
		Color = vec4(1.0,0.0,0.0,1.0); // FIXME
}
