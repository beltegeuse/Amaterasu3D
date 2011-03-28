#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D Grid; ///< Reprensent SH Grid
uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;

// Parametres
float CellDemiSize = 5.0;
float CellSizeFactor = 1.0/10.0;
float CellPrecision = 1.0/32.0;
float LPVNumberCellX = 8.0;
float LPVNumberCellY = 4.0;

// Sortie shader
smooth out vec2 outTexCoord;
smooth out vec3 outNormal;
invariant gl_Position;

vec3 ComputeGridCoordinates(vec4 Position)
{
	//TODO: Add boundary tests
	vec3 coords;
	coords.x = floor(Position.x*CellSizeFactor);
	coords.y = floor(Position.y*CellSizeFactor);
	coords.z = floor(Position.z*CellSizeFactor);
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
	outTexCoord.x = gl_Vertex.x;
	outTexCoord.y = gl_Vertex.y;

	// Get data
	vec4 Position = texture(PositionBuffer, outTexCoord);
	vec4 Normal = normalize(texture(NormalBuffer, outTexCoord));

	// Get Grid Coordinates
	vec3 GridCoords = ComputeGridCoordinates(Position+(Normal*CellDemiSize));

	outNormal = Normal.xyz;
	gl_Position = vec4(Map3DPosTo2D(GridCoords)*2.0-1.0,0.0,1.0);
}
