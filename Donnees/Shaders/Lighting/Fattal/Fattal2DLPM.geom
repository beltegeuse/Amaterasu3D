#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(points, max_vertices = 128) out; // TODO

// In Attributes values
in vec2 vOriPosition[1];
in vec2 vOriDirection[1];
in float vOriValue[1];

// Out (Offset modifications)
flat out float DeltaData;

// Precision qualifier
precision highp float;

// Texture data
uniform sampler2D UBuffer;
uniform sampler2D IBuffer;

// Grid information
uniform vec2 GridDimension;
uniform float AbsortionCoeff;
uniform float DiffusionCoeff;

// Other information
uniform vec2 MainDirection;

// Define const
#define BIAS 0.001

/////////////////////////////////
// Helper functions
/////////////////////////////////
{% include "ColorimetryHelper.shadercode" %}

// to know if the ray is in the volume
bool isInGrid(in vec2 voxID)
{
	return all(greaterThanEqual(voxID,vec2(0.0))) && all(lessThan(voxID, GridDimension));
}

float ReadU(in vec2 voxID, in vec2 mainDirection)
{
	vec4 data = texelFetch(UBuffer, ivec2(voxID), 0);
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
	vec2 Direction = normalize(vOriDirection[0]);
	vec2 DirectionAbs = abs(Direction);
	vec2 sDelta = sign(Direction);
	float maxDirectionCoord = max(DirectionAbs.x,DirectionAbs.y);
	
	// Data with position information
	vec2 Position = vOriPosition[0]+Direction*0.00001; // already mult by GridDimension
	vec2 voxWorldPos = floor(Position); //FIXME
	
	/////////////////////////////////
	// Initialisation tDelta
	/////////////////////////////////
	vec2 tDelta = vec2(100000);
	if(Direction.x != 0)
		tDelta.x = 1.0/abs(Direction.x);
	if(Direction.y != 0)
		tDelta.y = 1.0/abs(Direction.y);
	
	/////////////////////////////////
	// Initialise tMax
	/////////////////////////////////
	vec2 tMax = vec2(100000);
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
	
	////////////////////////////////
	// Loop (Ray martching )
	////////////////////////////////
	// Protection for reinjection steps
	bool isNeedRecast = true;
	int nbIntersection;
	int SumIntersection = 0;
	
	// Values of rays
	float rayValue = vOriValue[0];
	float OldCurrentLenght = 0.0;
	vec2 Offset = vec2(0.0);
	float Dist;
	
	// know the main direction
	bool xMainDirection = DirectionAbs.x > DirectionAbs.y;
	
	while(isNeedRecast)
	{
		isNeedRecast = false;
		nbIntersection = 0;
		while(true)
		{
			// Martch in the volume
			if(tMax.x < tMax.y)
			{
				Dist = tMax.x;
				tMax.x += tDelta.x;
			}
			else
			{
				Dist = tMax.y;
				tMax.y += tDelta.y;
			}
			
			// Get the length & Update OldCurrentLenght for next loop
			float DiffLength = Dist - OldCurrentLenght;
			OldCurrentLenght = Dist;

			// Update variables
			vec2 NewPosition = vOriPosition[0] + (Dist)*Direction + Offset;
			voxWorldPos = floor(NewPosition); // Be careful
			if(!isInGrid(voxWorldPos))
			{
				break;
			}
				
			Position = NewPosition;

			// Compute
			float scatteringTerm = rayValue*(1 - exp(-1*DiffLength*DiffusionCoeff/maxDirectionCoord));
			float extinctionCoeff = (DiffusionCoeff+AbsortionCoeff);
			float extinctionFactor = exp(-1*DiffLength*extinctionCoeff/maxDirectionCoord);
			float UValue = ReadU(voxWorldPos, MainDirection);
			rayValue = rayValue*extinctionFactor+(UValue*(1 - extinctionFactor)/extinctionCoeff);
			
			// Emit new values
			//TODO: NbRay
			//TODO: Area
			//TODO: CellVolume inverse
			//rayValue;//1.0*1.0*(3.14/(9))*scatteringTerm;
			DeltaData = 1.0*1.0*(3.14/(9))*scatteringTerm;
			gl_Position = vec4(((Position/GridDimension)*2 - 1)*1,0.0,1.0);
			EmitVertex();

			// Protection
			nbIntersection++;
		}
		
		SumIntersection += nbIntersection;
		// Protection for looping
		//FIXME
		if(nbIntersection == 0 || SumIntersection > 127) // < if no intersection => Quit
			break;
		
		// Compute relooping :)
		if(xMainDirection)
		{
			if(abs(Position.y) <= 0.01 || abs(Position.y-GridDimension.y) <= 0.01 )
			{
				Position -= Offset;
				isNeedRecast = true;
				if(sDelta.y == -1)
					Offset.y += GridDimension.y;
				else
					Offset.y -= GridDimension.y;
			}
		}
		else
		{
			if(abs(Position.x) <= 0.01 || abs(Position.x-GridDimension.x) <= 0.01)
			{
				Position -= Offset;
				isNeedRecast = true;
				if(sDelta.x == -1)
					Offset.x += GridDimension.x;
				else
					Offset.x -= GridDimension.x;
			}
		}
		EndPrimitive();
		Position += Offset;

		// Reinitialise rayValue
		//rayValue = 0;
	}
}
