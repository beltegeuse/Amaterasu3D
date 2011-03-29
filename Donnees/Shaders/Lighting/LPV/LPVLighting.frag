#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D Grid; ///< Reprensent SH Grid
uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;

// Parametres
uniform vec3 GridPosition;
float CellDemiSize = 5.0;
float CellSizeFactor = 1.0/10.0;
float CellPrecision = 1.0/32.0;
float LPVNumberCellX = 8.0;
float LPVNumberCellY = 4.0;

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

vec3 ComputeGridCoordinates(vec4 Position)
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
	float Xz = floor(GridCoords.z / LPVNumberCellX);
	float Yz = GridCoords.z - (Xz*LPVNumberCellX);
	vec2 Offset = vec2(Xz*(1.0/LPVNumberCellX),Yz*(1.0/LPVNumberCellY));
	vec2 SubCoord = vec2(GridCoords.x*CellPrecision,GridCoords.y*CellPrecision);
	return vec2(Offset.x+SubCoord.x,Offset.y+SubCoord.y);
}

void main()
{	
	// Get data
	vec4 Position = texture(PositionBuffer, outTexCoord);
	vec4 Normal = normalize(texture(NormalBuffer, outTexCoord));

	// Get Grid Coordinates FIXME
	vec3 GridCoords = ComputeGridCoordinates(Position+Normal); //+(Normal*CellDemiSize)
	vec2 TexCoordGrid = Map3DPosTo2D(GridCoords);
	vec4 CoeffGrid = texture2D(Grid, TexCoordGrid);
	if(CoeffGrid == vec4(0,0,0,1))
		Color = CoeffGrid;
	else
		Color = vec4(1.0,0.0,0.0,1.0); // FIXME
}
