#version 130

// Precision qualifier
precision highp float;

// In
smooth in vec2 outTexCoord;

// Texture
uniform vec4 UBuffer;
uniform vec4 IBuffer;
uniform vec4 DeltaIBuffer;
uniform vec4 DeltaUBuffer;

// Information
uniform vec2 MainDirection;

// Out buffers
out vec4 outUBuffer;
out vec4 outIBuffer;

void main()
{
	// Read data
	float vDeltaI = texture(DeltaIBuffer, outTexCoord).a;
	vec4 vDeltaU = texture(DeltaUBuffer, outTexCoord);
	float vI = texture(IBuffer, outTexCoord);
	vec4 vU = texture(UBuffer, outTexCoord);
	
	// Update I
	outIBuffer = vI + vDeltaI;
	
	// Update U
	// * Empty main direction
	if(MainDirection.x == -1)
		vU.x = 0;
	else if(MainDirection.x == 1)
		vU.y = 0;
	else if(MainDirection.y == -1)
		vU.z = 0;
	else
		vU.w = 0;
	// * Add values
	outUBuffer = vU + vDeltaU;
}
