/*
 * DeferredLighting.h
 *
 *  Created on: Mar 14, 2011
 *      Author: Adrien
 */

#ifndef DEFERREDLIGHTING_H_
#define DEFERREDLIGHTING_H_

#include <Graphics/Lighting/LightingPassAbstract.h>

class DeferredLighting : public LightingPassAbstract
{
public:
	DeferredLighting();
	virtual ~DeferredLighting();

	virtual void ComputeIllumination();
};

#endif /* DEFERREDLIGHTING_H_ */
