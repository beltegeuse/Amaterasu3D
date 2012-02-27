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

#ifndef ILIGHTSCENENODE_H_
#define ILIGHTSCENENODE_H_

#include <Graphics/SceneNode/ISceneNode.h>
#include "glm/glm.hpp"
#include <Graphics/Color.h>

namespace ama3D
{
class ILightSceneNode: public ISceneNode
{
public:
	/*
	 * Public enum
	 */
	enum LIGHT_TYPE
	{
		SPOT_LIGHT,
		SUN_LIGHT,
		POINT_LIGHT
	};
protected:
	/*
	 * Attributes
	 */
	LIGHT_TYPE m_LightType;
	Color m_Color;
	float m_LightIntensity;
	float m_LightRaduis;
	float m_LightCutOff;
public:
	/*
	 * Constructor & Destructors
	 */
	ILightSceneNode(const std::string& name, ISceneNode* parent);
	virtual ~ILightSceneNode();

	/*============
	 * Light propriety
	 *============= */
	// Setters
	void SetColor(const Color& color);
	void SetIntensity(float intensity);
	void SetRaduis(float raduis);
	void SetCutoff(float cutoff);
	void SetLightType(LIGHT_TYPE type);
	// Getters
	const Color& GetColor() const;
	float GetIntensity() const;
	float GetRaduis() const;
	float GetCutoff() const;
	LIGHT_TYPE GetLightType() const;

};
}
#endif /* ILIGHTSCENENODE_H_ */
