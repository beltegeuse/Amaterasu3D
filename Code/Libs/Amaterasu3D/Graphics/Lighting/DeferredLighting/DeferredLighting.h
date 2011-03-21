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
#include <Graphics/Window.h>

class DeferredLighting : public LightingPassAbstract
{
private:
	// Attributs
	TShaderPtr m_point_light_shader;
	TShaderPtr m_spot_light_shader;
	TShaderPtr m_simple_shader;
	bool m_debug_mode;
	Window* m_window;
public:
	DeferredLighting(Window* win);
	virtual ~DeferredLighting();

	virtual void ComputeIllumination();

	// DebugMode
	bool isDebugMode() const;
	void SetDebugMode(bool v);
protected:
	void PointLightPass();
	void SpotLightPass();
};

#endif /* DEFERREDLIGHTING_H_ */