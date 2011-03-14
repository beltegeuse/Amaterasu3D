/*
 * LightingPassAbstract.h
 *
 *  Created on: Mar 13, 2011
 *      Author: Beltegeuse
 */

#ifndef LIGHTINGPASSABSTRACT_H_
#define LIGHTINGPASSABSTRACT_H_

#include <Graphics/FBO.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/Lighting/LightingStructures.h>

#include <vector>

class LightingPassAbstract
{
protected:
	// Attributs
	std::vector<PointLight> m_points_light;
	FBO* m_FBO_graphics;
public:
	// Constructeur et destructeur
	LightingPassAbstract();
	virtual ~LightingPassAbstract();

	// public methods
	void SetFBOGraphicBuffer(FBO* fbo);
	void AddPointLight(const PointLight& light);

	// to compute all lightPass
	// need FBO of the scene
	virtual void ComputeIllumination() = 0;
};

#endif /* LIGHTINGPASSABSTRACT_H_ */
