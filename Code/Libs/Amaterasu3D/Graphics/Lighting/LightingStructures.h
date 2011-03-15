/*
 * LightingStructures.h
 *
 *  Created on: Mar 13, 2011
 *      Author: Beltegeuse
 */

#ifndef LIGHTINGSTRUCTURES_H_
#define LIGHTINGSTRUCTURES_H_

#include <Math/Vector3.h>
#include <Graphics/Color.h>

//XXX Add heritage structure

struct PointLight
{
	Math::TVector3F Position;
	Color LightColor;
	float LightRaduis;
	float LightIntensity;
};

struct SpotLight
{
	Math::TVector3F Position;
	Math::TVector3F Direction;
	Color LightColor;
	float LightIntensity;
	float LightRaduis;
	float LightCutOff;

};

#endif /* LIGHTINGSTRUCTURES_H_ */
