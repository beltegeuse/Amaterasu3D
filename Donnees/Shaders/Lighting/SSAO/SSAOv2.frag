#version 130

/*
 * Based on http://3d.benjamin-thaut.de/?p=16 Implementation.
 */

// Precision qualifier
precision highp float;

uniform sampler2D DepthBuffer;

uniform float NearClipping;
uniform float FarClipping;
uniform float ScreenWidth;
uniform float ScreenHeight;

smooth in vec2 outTexCoord;

out vec4 AmbiantOcculsion;

float readDepth(vec2 coord ) {
	return (2.0 * NearClipping) / (FarClipping + NearClipping - texture( DepthBuffer, coord ).r * (FarClipping - NearClipping));
}

float compareDepths( in float depth1, in float depth2 ) {
	float aoCap = 0.7;
	float aoMultiplier=10000.0;
	float depthTolerance=0.00005;
	float aorange = 20.0;// units in space the AO effect extends to (this gets divided by the camera far range)
	float diff = sqrt( clamp(1.0-(depth1-depth2) / (aorange/(FarClipping-NearClipping)),0.0,1.0) );
	float ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;
	return ao;
}

void main(void)
{
	float depth = readDepth( outTexCoord );
	float d;

	float pw = 1.0 / ScreenWidth;
	float ph = 1.0 / ScreenHeight;

	float aoCap = 1.0;

	float ao = 0.0;

	float aoMultiplier=10000.0;

	float depthTolerance = 0.001;

	float aoscale=1.0;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	pw*=2.0;
	ph*=2.0;
	aoMultiplier/=2.0;
	aoscale*=1.2;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y+ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x+pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	d=readDepth( vec2(outTexCoord.x-pw,outTexCoord.y-ph));
	ao+=compareDepths(depth,d)/aoscale;

	ao/=16.0;

	AmbiantOcculsion = vec4(vec3(1.0-ao),1.0);
}