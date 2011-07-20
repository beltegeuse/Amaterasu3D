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
	float TotalLength = length(Direction);
	Direction = normalize(Direction);
	vec3 Position = Entree*GridDimension;
	vec3 voxWorldPos = floor(Position);
	vec3 voxID = voxWorldPos;

	// tMax initialisation
	vec3 tMax = vec3(100000);
	if(Direction.x < 0)
		tMax.x = (voxWorldPos.x - Position.x) / Direction.x;
	else if(Direction.x > 0)
		tMax.x = (voxWorldPos.x + 1.0 - Position.x) / Direction.x;

	if(Direction.y < 0)
		tMax.y = (voxWorldPos.y - Position.y) / Direction.y;
	else if(Direction.y > 0)
		tMax.y = (voxWorldPos.y + 1.0 - Position.y) / Direction.y;

	if(Direction.z < 0)
		tMax.z = (voxWorldPos.z - Position.z) / Direction.z;
	else if(Direction.z > 0)
		tMax.z = (voxWorldPos.z + 1.0 - Position.z) / Direction.z;

	// Initialisation tDelta
	vec3 tDelta = vec3(100000);
	if(Direction.x != 0)
		tDelta.x = 1.0/abs(Direction.x);
	if(Direction.y != 0)
		tDelta.y = 1.0/abs(Direction.y);
	if(Direction.z != 0)
		tDelta.z = 1.0/abs(Direction.z);

	vec3 sDelta = sign(Direction);
	
	// Loop variables
	float OldCurrentLenght = 0.0;
	float Res = 0.0;
	int i = 0;
	while(i < 1000) //all(greaterThanEqual(voxID, vec3(0.0))) && all(lessThan(voxID, GridDimension))
	{
		vec3 diff;
		if(tMax.x < tMax.y && tMax.x < tMax.z)
		{
			diff = tMax.x*Direction;
			tMax.x += tDelta.x;
			voxID.x += sDelta.x;
		}
		else if(tMax.y < tMax.z)
		{
			diff = tMax.y*Direction;
			tMax.y += tDelta.y;
			voxID.y += sDelta.y;
		}
		else
		{
			diff = tMax.z*Direction;
			tMax.z += tDelta.z;
			voxID.z += sDelta.z;
		}
		float TravelLength = length(diff);
		float DiffLength = TravelLength - OldCurrentLenght;
		OldCurrentLenght = TravelLength;
		i++;
		
		// Read data
		//if(GridInterpolation)
		Res += texture(VolumeBuffer, Sample3DTexCoord2D(floor(Position+diff))).a*DiffLength;
		//else
		//Res += TrilinearInterpolation(VolumeBuffer, Position+diff).a*DiffLength;
	
		// Break conditions
		if(Res >= 1.f || TravelLength >= TotalLength)
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
