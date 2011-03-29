#version 130

// Precision qualifier
precision highp float;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

// Textures
uniform sampler2D Grid; ///< Reprensent SH Grid

// Parametres
float CellDemiSize = 5.0;
float CellSizeFactor = 1.0/10.0;
float CellPrecision = 1.0/32.0;
float LPVNumberCellX = 8.0;
float LPVNumberCellY = 4.0;

// Sortie shader
smooth out vec4 Value;

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

vec3 ComputeRealGridCoordinates(vec3 Position)
{
	vec3 coords;
	coords.x = Position.x/CellSizeFactor;
	coords.y = Position.y/CellSizeFactor;
	coords.z = Position.z/CellSizeFactor;
	return coords;
}

vec3 Map2DPosTo3D(vec2 TexCoord)
{
	vec3 res;
	float CellSize = 1/CellPrecision;
	// Compute to find Z
	vec2 indiceZ = floor(TexCoord*vec2(LPVNumberCellX, LPVNumberCellY));
	res.z = indiceZ.x*LPVNumberCellX+indiceZ.y;
	// Compute to Find XY
	vec2 Offset = TexCoord - indiceZ*vec2(1.0/LPVNumberCellX,1.0/LPVNumberCellY);
	res.xy = Offset*CellSize;
	return res;
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
	vec4 GridValue = texture(Grid,gl_Vertex.xy);
	if(GridValue != vec4(0.0))
		Value=vec4(1.0,0.0,0.0,1.0);
	vec4 Position = vec4(ComputeRealGridCoordinates(gl_Vertex.xyz*vec3(256.0,128.0,1.0)),1.0);
	gl_Position = Position;
}
