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
flat in vec2 vOriPosition;
flat in vec2 vOriDirection;

// Grid information
uniform vec2 GridDimension;
uniform float AbsortionCoeff;
uniform float DiffusionCoeff;

// Out (Offset modifications)
flat out float DeltaData;

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

vec2 ComputeMainDirectionRay(in vec2 DirectionAbs, in vec2 sDelta)
{
	if(DirectionAbs.x > DirectionAbs.y)
		return vec2(sDelta.x,0.0);
	else
		return vec2(0.0, sDelta.y);
}

float ReadU(in vec2 voxID, in vec2 mainDirection)
{
	vec4 data = texture(UBuffer, voxID/GridDimension);
	if(mainDirection.x == -1)
		return data.x;
	else if(mainDirection.x == 1)
		return data.y;
	else if(mainDirection.y == -1)
		return data.z;
	else
		return data.w;
}

void main()
{
	// Constraints
	//  * Cell size = 1

	/////////////////////////////////
	// Get data
	/////////////////////////////////
	// Data with direction information
	vec2 Direction = normalize(vOriDirection);
	vec2 DirectionAbs = abs(Direction);
	vec2 sDelta = sign(Direction);
	float maxDirectionCoord = max(DirectionAbs.x,DirectionAbs.y);
	vec2 MainDirection = ComputeMainDirectionRay(DirectionAbs, sDelta);
	// Data with position information
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
	
	////////////////////////////////
	// Loop (Ray martching )
	////////////////////////////////
	// Protection for reinjection steps
	bool isNeedRecast = true;
	int nbIntersection;
	// Values of rays
	int rayValue = 0;
	float OldCurrentLenght = 0.0;
	vec2 CurrentVoxID;
	// know the main direction
	bool xMainDirection = DirectionAbs.x > DirectionAbs.y;
	
	while(isNeedRecast)
	{
		isNeedRecast = false;
		nbIntersection = 0;
		while(isInGrid(voxWorldPos))
		{
			// Save the current VoxID for cell reading
			CurrentVoxID = voxWorldPos; // Need +1 ?
			
			// Martch in the volume
			vec2 diff;
			if(tMax.x < tMax.y)
			{
				diff = tMax.x*Direction;
				tMax.x += tDelta.x;
				voxWorldPos.x += sDelta.x;
			}
			else
			{
				diff = tMax.y*Direction;
				tMax.y += tDelta.y;
				voxWorldPos.y += sDelta.y;
			}
			
			// Get the length & Update OldCurrentLenght for next loop
			float TravelLength = length(diff);
			float DiffLength = TravelLength - OldCurrentLenght;
			OldCurrentLenght = TravelLength;
			
			// Compute
			float scatteringTerm = rayValue*(1 - exp(-1*DiffLength*I.S/maxDirectionCoord));
			float extinctionCoeff = (DiffusionCoeff+AbsortionCoeff);
			float extinctionFactor = exp(-1*dist*extinctionCoeff/maxDirectionCoord);
			float UValue = ReadU(CurrentVoxID, MainDirection);
			rayValue = rayValue*extinctionFactor+(UValue*(1 - extinctionFactor)/extinctionCoeff);
			
			// Emit new values
			//TODO: NbRay
			//TODO: Area
			//TODO: CellVolume inverse
			DeltaData = 1.0*1.0*(3.14/(9))*scatteringTerm;
			gl_Position = vec4(CurrentVoxID/GridDimension,0.0,1.0);
			EmitVertex();
			
			// Protection
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
