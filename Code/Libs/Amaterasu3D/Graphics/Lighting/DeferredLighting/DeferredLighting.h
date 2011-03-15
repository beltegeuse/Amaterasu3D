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
	bool m_debug_mode;
public:
	DeferredLighting();
	virtual ~DeferredLighting();

	virtual void ComputeIllumination();

	// DebugMode
	bool isDebugMode() const;
	void SetDebugMode(bool v);
protected:
	void PointLightPass();
};

#endif /* DEFERREDLIGHTING_H_ */
