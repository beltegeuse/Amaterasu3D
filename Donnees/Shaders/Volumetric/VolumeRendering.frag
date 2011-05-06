#version 130

// Precision qualifier
precision highp float;

// Textures
uniform sampler2D FrontBuffer;
uniform sampler2D BackBuffer;
uniform sampler2D VolumeBuffer;

// Grid information
uniform vec3 GridDimension;
uniform vec4 GridTextureSize; //xy texture size && zw Repeat texture part
uniform bool GridInterpolation;
//TODO: Add grid transformationn to know the grid position

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

#define MaxFactor 2.82842712 // 2*sqrt(2)

vec2 Sample3DTexCoord2D(vec3 Position)
{
	float row = floor(Position.z / GridTextureSize.z);
	float col = Position.z - (row*GridTextureSize.z);
	return vec2(col * GridDimension.z + Position.x, row * GridDimension.z + Position.y) / GridTextureSize.xy;
}

vec4 TrilinearInterpolation(sampler2D s, vec3 Position)
{
	vec3 IndexedPosition = floor(Position);
	vec3 Offset = Position - IndexedPosition;

	vec4 v1i1 = texture(s, Sample3DTexCoord2D(IndexedPosition));
	vec4 v2i1 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(0,0,1)));
	vec4 i1 = v1i1*(1-Offset.z)+v2i1*Offset.z;

	vec4 v1i2 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(0,1,0)));
	vec4 v2i2 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(0,1,1)));
	vec4 i2 = v1i2*(1-Offset.z)+v2i2*Offset.z;

	vec4 v1j1 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(1,0,0)));
	vec4 v2j1 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(1,0,1)));
	vec4 j1 = v1j1*(1-Offset.z)+v2j1*Offset.z;

	vec4 v1j2 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(1,1,0)));
	vec4 v2j2 = texture(s, Sample3DTexCoord2D(IndexedPosition + vec3(1,1,1)));
	vec4 j2 = v1j2*(1-Offset.z)+v2j2*Offset.z;

	vec4 w1 = i1*(1 - Offset.y) + i2 * Offset.y;
	vec4 w2 = j1*(1 - Offset.y) + j2 * Offset.y;

	return w1*(1 - Offset.x) + w2 * Offset.x;
}

float GetVolumeData(vec3 Position)
{
	return texture(VolumeBuffer, Sample3DTexCoord2D(floor(Position))).a;
}

float GetVolumeDataTri(vec3 Position)
{
	return TrilinearInterpolation(VolumeBuffer, Position).a;
}


bool isOccupency(vec3 Position)
{
	return GetVolumeData(Position) > 0.0;
}

void RayMarching(vec3 Entree, vec3 Sortie)
{
	// Get the ray direction
	vec3 Direction = (Sortie - Entree)*GridDimension;
	float Length = length(Direction);
	Direction = normalize(Direction);
	
	// Initialisation for the loop
	vec3 Position = Entree*GridDimension;
	int NbIteration = int(GridDimension.x*MaxFactor); // FIXME: Normally the biggest dimension
	
	// Loop variables
	int i = 0;
	float CurrentLenght = 0.0;
	float Alpha = 0.0;
	for(; i <= NbIteration; i++)
	{
		if(CurrentLenght > Length) break;
		
		Position += Direction;
		CurrentLenght += 1.0;
		
		if(GridInterpolation)
			Alpha += (GetVolumeDataTri(Position)/(GridDimension.x));
		else
			Alpha += (GetVolumeData(Position)/(GridDimension.x));

		if(Alpha > 1.0)
		{
			break;
		}
	}
	
	Color = vec4 (min(vec3(Alpha)*2.0,1.0),1.0);
}

void main()
{	
	vec3 Front = texture2D(FrontBuffer, outTexCoord).rgb;
	vec3 Back = texture2D(BackBuffer, outTexCoord).rgb;
	
	if(all(equal(Back,vec3(0.0)))) // Miss the volume
	{
		Color = vec4 (0.0);
	}
	else
	{
		if(all(equal(Front,vec3(0.0)))) // In the volume
		{
			Color = vec4 (1.0);
		}

		RayMarching(Front,Back);
	}
}
