#version 150
#extension GL_EXT_geometry_shader4 : enable

{% if DEBUG_MODE %}
// Puts in Debug Mode! DEBUG_MODE : {{DEBUG_MODE}}
layout(points) in;
layout(line_strip, max_vertices = 128) out;
{% else %}
// Puts in Normal Mode! DEBUG_MODE : {{DEBUG_MODE}}
layout(points) in;
layout(points, max_vertices = 128) out;
{% endif %}

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
uniform vec2 CellDimension;
uniform float AbsortionCoeff;
uniform float DiffusionCoeff;
uniform bool isFristSweep;
// Other information
uniform vec2 MainDirection;


// Define const
#define BIAS 0.001

/////////////////////////////////
// Helper functions
/////////////////////////////////
// Includes
{% include "ColorimetryHelper.shadercode" %}
{% include "Helpers/MainDirection.shadercode" %}

// to know if the ray is in the volume
bool isInGrid(in vec2 voxID)
{
	return all(greaterThanEqual(voxID,vec2(0.0))) && all(lessThan(voxID, GridDimension));
}

void main()
{
	// Constraints
	//  * Cell size = 1

	/////////////////////////////////
	// Get data
	/////////////////////////////////
	// Precompute data
	float CellVolume = CellDimension.x*CellDimension.y;
	vec2 GridWorldDimension = GridDimension*CellDimension;
	
	// Data with direction information
	vec2 Direction = normalize(vOriDirection[0]);
	vec2 DirectionAbs = abs(Direction);
	vec2 sDelta = sign(Direction);
	float maxDirectionCoord = max(DirectionAbs.x,DirectionAbs.y);
	
	// Data with position information
	vec2 Position = vOriPosition[0]+Direction*0.001; // already mult by GridDimension
	vec2 voxID = floor(Position/CellDimension);
	vec2 voxWorldPos = voxID*CellDimension;
	
	/////////////////////////////////
	// Initialisation tDelta
	/////////////////////////////////
	vec2 tDelta = vec2(100000);
	if(Direction.x != 0)
		tDelta.x = CellDimension.x/abs(Direction.x);
	if(Direction.y != 0)
		tDelta.y = CellDimension.y/abs(Direction.y);
	
	/////////////////////////////////
	// Initialise tMax
	/////////////////////////////////
	vec2 tMax = vec2(100000);
	// * x Initialisation
	if(Direction.x < 0)
		tMax.x = (voxWorldPos.x - Position.x) / Direction.x;
	else if(Direction.x > 0)
		tMax.x = (voxWorldPos.x + CellDimension.x - Position.x) / Direction.x;
	// * y Initialisation
	if(Direction.y < 0)
		tMax.y = (voxWorldPos.y - Position.y) / Direction.y;
	else if(Direction.y > 0)
		tMax.y = (voxWorldPos.y + CellDimension.y - Position.y) / Direction.y;
	
	////////////////////////////////
	// Loop (Ray martching )
	////////////////////////////////
	// Protection for reinjection steps
	bool isNeedRecast = true;
	int nbIntersection;
	int SumIntersection = 0;
	
	// Values of rays
	float rayValue;
	if(isFristSweep)
		rayValue = vOriValue[0];
	else
		rayValue = 0;
	
	// loop variables
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
			// March in the volume
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
			voxID = floor(NewPosition/CellDimension); // Be careful
			if(!isInGrid(voxID))
			{
				break;
			}
				
			Position = NewPosition;

			// Compute
			float scatteringTerm = rayValue*(1 - exp(-1*DiffLength*DiffusionCoeff/maxDirectionCoord));
			float extinctionCoeff = (DiffusionCoeff+AbsortionCoeff);
			float extinctionFactor = exp(-1*DiffLength*extinctionCoeff/maxDirectionCoord);
			float UValue = ReadU(UBuffer, voxID, MainDirection);
			rayValue = rayValue*extinctionFactor+(UValue*(1 - extinctionFactor)/extinctionCoeff);
			
			// Emit new values
			//TODO: NbRay
			{% if DEBUG_MODE %}
			DeltaData = 1.0;
			{% else %}
			if(xMainDirection)
				DeltaData = CellDimension.x*(1.0/CellVolume)*scatteringTerm*(3.14/(9));
			else
				DeltaData = CellDimension.y*(1.0/CellVolume)*scatteringTerm*(3.14/(9));
			{% endif %}
			
			gl_Position = vec4(((Position/GridWorldDimension)*2 - 1)*1,0.0,1.0);
			EmitVertex();
			
			{% if not DEBUG_MODE %}
			EndPrimitive();
			{% endif %}
			
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
			if(abs(Position.y) <= 0.01 || abs(Position.y-GridWorldDimension.y) <= 0.01 )
			{
				Position -= Offset;
				isNeedRecast = true;
				if(sDelta.y == -1)
					Offset.y += GridWorldDimension.y;
				else
					Offset.y -= GridWorldDimension.y;
			}
			{% if DEBUG_MODE %}
			else
			{
				DeltaData = 1.0;//1.0*1.0*(3.14/(9))*scatteringTerm;
				gl_Position = vec4(0.0,0.0,0.0,1.0);
				EmitVertex();
			}
			{% endif %}
		}
		else
		{
			if(abs(Position.x) <= 0.01 || abs(Position.x-GridWorldDimension.x) <= 0.01)
			{
				Position -= Offset;
				isNeedRecast = true;
				if(sDelta.x == -1)
					Offset.x += GridWorldDimension.x;
				else
					Offset.x -= GridWorldDimension.x;
			}
			{% if DEBUG_MODE %}
			else
			{
				DeltaData = 1.0;//1.0*1.0*(3.14/(9))*scatteringTerm;
				gl_Position = vec4(0.0,0.0,0.0,1.0);
				EmitVertex();
			}
			{% endif %}
		}
		Position += Offset;

		{% if DEBUG_MODE %}
		EndPrimitive();
		DeltaData = 1.0;//1.0*1.0*(3.14/(9))*scatteringTerm;
		gl_Position = vec4(((Position/GridWorldDimension)*2 - 1),0.0,1.0);
		EmitVertex();
		{% endif %}

		// Reinitialise rayValue
		rayValue = 0;
	}
}
