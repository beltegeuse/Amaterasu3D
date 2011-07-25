#version 130

// Precision qualifier
precision highp float;

// In
smooth in vec2 outTexCoord;

// Texture
uniform sampler2D UBuffer;
uniform sampler2D IBuffer;
uniform sampler2D DeltaIBuffer;
uniform sampler2D DeltaUBuffer;

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
	float vI = texture(IBuffer, outTexCoord).a;
	vec4 vU = texture(UBuffer, outTexCoord);
	
	// Update I
	outIBuffer = vec4(vI + vDeltaI);
	
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
