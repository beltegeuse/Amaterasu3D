#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(points, max_vertices = 128)out; // TODO

// Precision qualifier
precision highp float;

// Texture data
uniform sampler2D UBuffer;
uniform sampler2D IBuffer;

// In Attributes values
in flat vec2 vOriPosition;
in flat vec2 vOriDirection;

// Grid information
uniform vec2 GridDimension;

// Out (Offset modifications)
out flat vec4 DeltaData;

// Define const
#define BIAS 0.001

/////////////////////////////////
// Helper functions
/////////////////////////////////
// to know if the ray is in the volume
bool isInGrid(in vec2 voxID)
{
	return all(greaterThanEqual(voxID, vec2(0.0))) && all(lessThan(voxID, GridDimension));
}

void main()
{
	// Constraints
	//  * Cell size = 1

	/////////////////////////////////
	// Get data
	/////////////////////////////////
	vec2 Direction = normalize(vOriDirection);
	vec2 Position = vOriPosition; // already mult by GridDimension
	vec2 voxWorldPos = floor(Position);
	
	/////////////////////////////////
	// Initialise tMax
	/////////////////////////////////
	vec2 tMax = vec3(100000);
	// * x Initialisation
	if(Direction.x < 0)
		tMax.x = (voxWorldPos.x - Position.x) / Direction.x;
	else if(Direction.x > 0)
		tMax.x = (voxWorldPos.x + 1.0 - Position.x) / Direction.x;
	// * y Initialisation
	if(Direction.y < 0)
		tMax.y = (voxWorldPos.y - Position.y) / Direction.y;
	else if(Direction.y > 0)
		tMax.y = (voxWorldPos.y + 1.0 - Position.y) / Direction.y;

	/////////////////////////////////
	// Initialisation tDelta
	/////////////////////////////////
	vec2 tDelta = vec3(100000);
	if(Direction.x != 0)
		tDelta.x = 1.0/abs(Direction.x);
	if(Direction.y != 0)
		tDelta.y = 1.0/abs(Direction.y);
		
	vec2 sDelta = sign(Direction);
	
	////////////////////////////////
	// Loop (Ray martching )
	////////////////////////////////
	// Protection for reinjection steps
	bool isNeedRecast = true;
	int nbIntersection;
	// Values of rays
	int rayValue = 0;
	// know the main direction
	bool xMainDirection = abs(Direction.x) > abs(Direction.y);
	while(isNeedRecast)
	{
		isNeedRecast = false;
		nbIntersection = 0;
		while(isInGrid(voxWorldPos))
		{
			
			nbIntersection++;
		}
		
		// Protection for looping
		if(nbIntersection == 0) // < if no intersection => Quit
			break;
		
		// Compute relooping :)
		if(xMainDirection)
		{
			if(Position.y <= 0 || Position.y >= GridDimension.y)
			{
				isNeedRecast = true;
				if(sDeltas.y == -1)
					Position.y = GridDimension.y-BIAS;
				else
					Position.y = BIAS;
			}
		}
		else
		{
			if(Position.x <= 0 || Position.x >= GridDimension.x)
			{
				isNeedRecast = true;
				if(sDeltas.x == -1)
					Position.x = GridDimension.x-BIAS;
				else
					Position.x = BIAS;
			}
		}
		
		// Reinitialise rayValue
		rayValue = 0;
	}
}
