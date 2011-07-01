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

#include "ILightSceneNode.h"
namespace ama3D
{
ILightSceneNode::ILightSceneNode(const std::string& name, ISceneNode* parent) :
		ISceneNode(name, parent),
		m_LightType(ILightSceneNode::SPOT_LIGHT),
		m_Color(1,1,1,1),
		m_LightIntensity(1.f),
		m_LightRaduis(100.f),
		m_LightCutOff(90.f)
{
}

ILightSceneNode::~ILightSceneNode()
{
}

///////////////////
// Light proprity
///////////////////
void ILightSceneNode::SetColor(const Color& color)
{
	m_Color = color;
}
void ILightSceneNode::SetIntensity(float intensity)
{
	m_LightIntensity = intensity;
}
void ILightSceneNode::SetRaduis(float raduis)
{
	m_LightRaduis = raduis;
}
void ILightSceneNode::SetCutoff(float cutoff)
{
	m_LightCutOff = cutoff;
}
void ILightSceneNode::SetLightType(ILightSceneNode::LIGHT_TYPE type)
{
	m_LightType = type;
}

const Color& ILightSceneNode::GetColor() const
{
	return m_Color;
}
float ILightSceneNode::GetIntensity() const
{
	return m_LightIntensity;
}
float ILightSceneNode::GetRaduis() const
{
	return m_LightRaduis;
}
float ILightSceneNode::GetCutoff() const
{
	return m_LightCutOff;
}
ILightSceneNode::LIGHT_TYPE ILightSceneNode::GetLightType() const
{
	return m_LightType;
}

}
