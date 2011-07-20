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
//uniform bool GridInterpolation;
//TODO: Add grid transformationn to know the grid position

// To inverse the projections
uniform float FarClipping;
uniform float NearClipping;
uniform vec2 UnprojectInfo;
uniform mat4 InverseViewMatrix;

{% include 'GetPosition.shadercode' %}
{% include 'Trilinear.shadercode' %}

// Entree
smooth in vec2 outTexCoord;

// Sortie
out vec4 Color;

#define MaxFactor 2.82842712 // 2*sqrt(2)

void RayMarching(vec3 Entree, vec3 Sortie)
{
	// Get the ray direction
	vec3 Direction = (Sortie - Entree)*GridDimension;
	float Length = length(Direction);
	Direction = normalize(Direction);
	
	// Initialisation for the loop
	vec3 Position = Entree*GridDimension;
	int NbIteration = 0;

	if(GridDimension.x > GridDimension.y && GridDimension.x > GridDimension.z)
		NbIteration = int(GridDimension.x*MaxFactor)+1;
	else if(GridDimension.y > GridDimension.z )
		NbIteration = int(GridDimension.y*MaxFactor)+1;
	else
		NbIteration = int(GridDimension.z*MaxFactor)+1;
	
	// Loop variables
	int i = 0;
	float CurrentLenght = 0.0;
	float Res = 0.0;
	for(; i <= NbIteration; i++)
	{
		if(CurrentLenght > Length) break;
		
		Position += Direction;
		CurrentLenght += 1.0;
		
		// Read data
		//if(GridInterpolation)
		//Res += texture(VolumeBuffer, Sample3DTexCoord2D(floor(Position))).a;
		//else
		Res += TrilinearInterpolation(VolumeBuffer, Position).a;
	
		// Break conditions
		if(Res >= 1.f)
			break;
	}
	
	Color = vec4(vec3(Res),1.0);
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
			Front = PositionCameraPlane(outTexCoord).xyz / GridDimension;
		}

		RayMarching(Front,Back);
	}
}
