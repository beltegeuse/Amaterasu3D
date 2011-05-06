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

vec4 GetVolumeData(vec3 Position)
{
	return texture(VolumeBuffer, Sample3DTexCoord2D(floor(Position)));
}

bool isOccupency(vec3 Position)
{
	return any(greaterThan(GetVolumeData(Position).rgb,vec3(0.0)));
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
	
	for(; i <= NbIteration; i++)
	{
		if(CurrentLenght > Length) break;
		
		Position += Direction;
		CurrentLenght += 1.0;
		
		if(isOccupency(Position)) 
		{
			Color = vec4 (1.0);
			return;
		}
	}
	
	Color = vec4 (0.0);
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
		else
		{
			RayMarching(Front,Back);
		}
	}
}