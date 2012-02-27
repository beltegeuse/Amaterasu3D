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

#include "glm/ext.hpp"
#include "CameraAbstract.h"
#include <Graphics/MatrixManagement.h>

namespace ama3D
{
CameraAbstract::CameraAbstract(const  glm::vec3& pos,
		const  glm::vec3& target, const  glm::vec3& up) :
		m_Position(pos), m_Target(target), m_Up(up)
{
}

CameraAbstract::~CameraAbstract()
{
}

const glm::mat4x4& CameraAbstract::GetMatrix()
{
	return m_ViewMatrixTransform;
}

void CameraAbstract::SetMatrix(const glm::mat4x4& matrix)
{
	m_ViewMatrixTransform = matrix;
}

void CameraAbstract::ComputeMatrix()
{
	m_ViewMatrixTransform = glm::lookAt(m_Position, m_Target, glm::vec3(0,1,0));
}

void CameraAbstract::GetView()
{
	CMatrixManager::Instance().SetViewMatrix(m_ViewMatrixTransform);
}

const  glm::vec3& CameraAbstract::GetPosition() const
{
	return m_Position;
}

const  glm::vec3& CameraAbstract::GetTarget() const
{
	return m_Target;
}

void CameraAbstract::SetPosition(const  glm::vec3& position)
{
	m_Position = position;
}

void CameraAbstract::SetTarget(const  glm::vec3& target)
{
	m_Target = target;
}
}
