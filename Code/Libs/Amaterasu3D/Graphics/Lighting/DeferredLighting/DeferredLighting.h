/*
 * DeferredLighting.h
 *
 *  Created on: Mar 14, 2011
 *      Author: Adrien
 */

#ifndef DEFERREDLIGHTING_H_
#define DEFERREDLIGHTING_H_

#include <Graphics/Lighting/LightingPassAbstract.h>
#include <Graphics/GLSLShader.h>

class DeferredLighting : public LightingPassAbstract
{
private:
	// Attributs
	TShaderPtr m_point_light_shader;
public:
	DeferredLighting();
	virtual ~DeferredLighting();

	virtual void ComputeIllumination();
protected:
	void PointLightPass();
};

#endif /* DEFERREDLIGHTING_H_ */
