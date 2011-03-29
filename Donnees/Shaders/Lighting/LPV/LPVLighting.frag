#version 130

//this is c = vec4(c0,-c1,c1,-c1)
//with c0 = 1 / ( 2 * sqrt(PI))
//     c1 = sqrt(3) / ( 2 * sqrt(PI))
#define SH_c vec4(0.282094792,-0.488602512,0.488602512,0.488602512)
//#define SH_cosLobe_c vec4(0.886226925,-1.02332671,1.02332671,1.02332671)
#define SH_cosLobe_c vec4(0.25,-0.5,0.5,0.5)
//#define SH_cosLobe_c vec4(0.4,-0.8,0.8,0.8)

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

//Evaluates a SH
vec4 SH_evaluate(vec3 dir){
  return SH_c * vec4(1.0, dir.y, dir.z, dir.x);
}

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

	// Get Grid Coordinates
	Position = floor((Position-LPVPosition) / LPVCellSize.xyz);

	vec2 TexCoordGrid = Map3DPosTo2D(Position);
	vec4 CoeffGrid = texture2D(Grid, TexCoordGrid);
//	if(CoeffGrid == vec4(0,0,0,1))
//		Color = CoeffGrid;
//	else
//		Color = SH_evaluate(-Normal); // FIXME
	Color = CoeffGrid;
}
