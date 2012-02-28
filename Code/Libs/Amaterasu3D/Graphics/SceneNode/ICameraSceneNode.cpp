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

#include "ICameraSceneNode.h"
#include "glm/ext.hpp"
namespace ama3D
{
ICameraSceneNode::ICameraSceneNode(const std::string& name, ISceneNode* parent) :
		ISceneNode(name, parent)
{
}

ICameraSceneNode::~ICameraSceneNode()
{
}

// Projection management
// * General
const glm::mat4x4 ICameraSceneNode::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}
// * Setters
void ICameraSceneNode::SetFOV(float fov)
{
	m_FOV = fov;
}
void ICameraSceneNode::SetRatio(float ratio)
{
	m_Ratio = ratio;
}
void ICameraSceneNode::SetNear(float near)
{
	m_Near = near;
}
void ICameraSceneNode::SetFar(float far)
{
	m_Far = far;
}
// * Getters
float ICameraSceneNode::GetFOV() const
{
	return m_FOV;
}
float ICameraSceneNode::GetRatio() const
{
	return m_Ratio;
}
float ICameraSceneNode::GetNear() const
{
	return m_Near;
}
float ICameraSceneNode::GetFar() const
{
	return m_Far;
}
void ICameraSceneNode::UpdateTransformations()
{
	ISceneNode::UpdateTransformations();
	if (m_NeedUpdateProjectionMatrix)
	{
		m_ProjectionMatrix = glm::perspectiveFov(m_FOV,
				m_Ratio, 1/m_Ratio, m_Near, m_Far); // ????
		m_NeedUpdateProjectionMatrix = false;
	}
}
void ICameraSceneNode::NeedProjectionUpdate()
{
	if (!m_NeedUpdateProjectionMatrix)
	{
		m_NeedUpdateProjectionMatrix = true;
		NeedUpdate();
	}
}
}
