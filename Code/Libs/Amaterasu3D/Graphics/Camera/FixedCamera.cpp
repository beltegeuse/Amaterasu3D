/*
 * FixedCamera.cpp
 *
 *  Created on: May 27, 2011
 *      Author: adrien
 */

#include "FixedCamera.h"

FixedCamera::FixedCamera(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up) :
	CameraAbstract(pos, target, up)
{
}

FixedCamera::~FixedCamera()
{
}
