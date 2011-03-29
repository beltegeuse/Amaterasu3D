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

vec2 Convert3Dto2D(in vec3 pos){
    // Cacul les coordonnes de la sous image
	float row = floor(pos.z / LPVSize.z);
	float col = pos.z - (row*LPVSize.z);

	// Check is right Boundaries
	vec2 over = (pos.xy - clamp(pos.xy,vec2(0,0),vec2(LPVCellSize.w-1.0,LPVCellSize.w-1.0))) * 100.0; // 100.0 ? nbCell ?

	return vec2(col * LPVCellSize.w + pos.x + over.x * LPVSize.x,row * LPVCellSize.w + pos.y + over.y * LPVSize.y);
}


void main()
{	
	outTexCoord.x = gl_Vertex.x;
	outTexCoord.y = gl_Vertex.y;

	// Get data
	vec3 Position = texture(PositionBuffer, outTexCoord).xyz;
	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	Position += (Normal*LPVCellSize.xyz*0.5);

	Position = floor((Position-LPVPosition) / LPVCellSize.xyz);

	outNormal = Normal.xyz;
	vec2 pos2d = Convert3Dto2D(Position) / LPVSize.xy;
	gl_Position = vec4(pos2d*2.0-1.0,0.0,1.0);
}
