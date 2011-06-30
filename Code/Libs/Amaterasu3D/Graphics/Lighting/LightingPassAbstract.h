//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================

#ifndef LIGHTINGPASSABSTRACT_H_
#define LIGHTINGPASSABSTRACT_H_

#include <Graphics/FBO.h>
#include <Graphics/Lighting/LightingStructures.h>

#include <vector>

class LightingPassAbstract
{
protected:
	// Attributs
	// * Differents type of light
	std::vector<PointLight> m_points_lights;
	std::vector<SpotLight> m_spots_lights;
	// * FBO for the GBuffer acces
	FBO* m_FBO_graphics;

public:
	// Constructeur et destructeur
	LightingPassAbstract();
	virtual ~LightingPassAbstract();

	// public methods
	void SetFBOGraphicBuffer(FBO* fbo);
	void AddPointLight(const PointLight& light);
	void AddSpotLight(const SpotLight& light);

	// to compute all lightPass
	// need FBO of the scene
	virtual void ComputeIllumination() = 0;
};

#endif /* LIGHTINGPASSABSTRACT_H_ */
