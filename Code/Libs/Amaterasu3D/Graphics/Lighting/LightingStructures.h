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

struct PointLight
{
	Math::TVector3F Position;
	Color LightColor;
};

#endif /* LIGHTINGSTRUCTURES_H_ */
