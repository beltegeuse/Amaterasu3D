#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D GridRed;
uniform sampler2D GridGreen; ///< Reprensent SH Grid
uniform sampler2D GridBlue;

uniform sampler2D DepthBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DiffuseBuffer;

// Parametres
uniform mat4 LPVMatrix; // position of the grid
uniform vec4 LPVSize; // xy : texture dim & zw : repeat.
uniform vec4 LPVCellSize; // xyz dim & w number cell in one dim
uniform bool EnableTrilinearInterpolation;

// Parameters to compute position form Depth
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

#include <GetPosition.shadercode>

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

#include <LPVSH.shadercode>
#include <LPVPosition.shadercode>

// Trilinear interpolation
// \position : It's grid position
//TODO: Add border support
vec4 TrilinearInterpolation(sampler2D s, vec3 Position, vec3 Normal)
{
	vec3 IndexedPosition = floor(Position);
	vec3 Offset = Position - IndexedPosition;

	vec4 v1i1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition));
	vec4 v2i1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,0,1)));
	vec4 i1 = v1i1*(1-Offset.z)+v2i1*Offset.z;

	vec4 v1i2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,1,0)));
	vec4 v2i2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(0,1,1)));
	vec4 i2 = v1i2*(1-Offset.z)+v2i2*Offset.z;

	vec4 v1j1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,0,0)));
	vec4 v2j1 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,0,1)));
	vec4 j1 = v1j1*(1-Offset.z)+v2j1*Offset.z;

	vec4 v1j2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,1,0)));
	vec4 v2j2 = texture(s, Convert3DTo2DTexcoord(IndexedPosition + vec3(1,1,1)));
	vec4 j2 = v1j2*(1-Offset.z)+v2j2*Offset.z;

	vec4 w1 = i1*(1 - Offset.y) + i2 * Offset.y;
	vec4 w2 = j1*(1 - Offset.y) + j2 * Offset.y;

	return w1*(1 - Offset.x) + w2 * Offset.x;
}

vec4 TrilinearInterpolationWorld(sampler2D s, vec3 Position, vec3 Normal)
{
	vec3 PositionGrid = ((LPVMatrix*vec4(Position - 0.5*LPVCellSize.xyz,1.0)).xyz / LPVCellSize.xyz);
	return TrilinearInterpolation(s,PositionGrid, Normal);
}

vec4 sample3D(in sampler2D s, in vec3 coords){
  float CellCount = (LPVCellSize.w-1.0);
  vec3 c = clamp(coords,0.0,1.0) * CellCount;

  vec3 c1 = vec3(c.x,c.y,floor(c.z));
  vec3 c2 = clamp(c1+vec3(0.0,0.0,1.0),0.0,CellCount);

	float row = floor(c1.z / LPVSize.z);
	float col = c1.z - (row*LPVSize.z);
	vec2 spos1 = vec2(col * LPVCellSize.w + c1.x,row * LPVCellSize.w + c1.y) / LPVSize.xy;

	row = floor(c2.z / LPVSize.z);
	col = c2.z - (row*LPVSize.z);
	vec2 spos2 = vec2(col * LPVCellSize.w + c2.x,row * LPVCellSize.w + c2.y) / LPVSize.xy;

	vec4 s1 = texture2D(s,spos1);
	vec4 s2 = texture2D(s,spos2);

	return mix(s1,s2,c.z-c1.z);
}

void main()
{	
	// Get data from buffers
	vec3 Position = PositionFormDepth(DepthBuffer, outTexCoord).xyz;
	vec3 GridPosition = floor((LPVMatrix*vec4(Position.xyz,1.0)).xyz / LPVCellSize.xyz); // Early discard
	if(IsNotInGrid(GridPosition))
	{
		discard;
	}

	vec3 Normal = normalize(texture(NormalBuffer, outTexCoord).xyz * 2.0 - 1.0);
	vec4 DiffuseColor = texture(DiffuseBuffer,outTexCoord);

	vec4 CoeffGridRed;
	vec4 CoeffGridGreen;
	vec4 CoeffGridBlue;

	if(EnableTrilinearInterpolation)
	{
		CoeffGridRed = TrilinearInterpolationWorld(GridRed,Position, Normal);
		CoeffGridGreen = TrilinearInterpolationWorld(GridGreen,Position,Normal);
		CoeffGridBlue = TrilinearInterpolationWorld(GridBlue,Position, Normal);
	}
	else
	{

		// Get texture coordinates
		vec2 TexCoordGrid = Convert3DTo2DTexcoord(GridPosition);
		CoeffGridRed = texture2D(GridRed, TexCoordGrid); ///< And get coeff value
		CoeffGridGreen = texture2D(GridGreen, TexCoordGrid);
		CoeffGridBlue = texture2D(GridBlue, TexCoordGrid);
	}

	vec4 SHEv = SH_evaluate(-Normal);
	Color =  vec4(clamp(4.0*dot(CoeffGridRed,SHEv),0,1),
				clamp(4.0*dot(CoeffGridGreen,SHEv),0,1),
				clamp(4.0*dot(CoeffGridBlue,SHEv),0,1),1.0); //
	//Color = CoeffGrid;
}
