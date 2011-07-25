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
	// TODO: Need to do the reinjection steps :)
	while(isInGrid(voxWorldPos))
	{
		
	}
}
