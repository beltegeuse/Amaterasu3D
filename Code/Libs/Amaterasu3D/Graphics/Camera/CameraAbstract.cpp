//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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

#include "CameraAbstract.h"
#include <Graphics/GLSLShader.h>
#include <Graphics/MatrixManagement.h>

CameraAbstract::CameraAbstract(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up) :
	m_position(pos),
	m_target(target),
	m_up(m_up)
{
}

CameraAbstract::~CameraAbstract()
{
}

const Math::CMatrix4& CameraAbstract::GetMatrix()
{
//	std::cout << m_matrix << std::endl;
	return m_matrix;
}

void CameraAbstract::SetMatrix(const Math::CMatrix4& matrix)
{
	m_matrix = matrix;
}

void CameraAbstract::ComputeMatrix(double delta)
{
//	std::cout << "[DEBUG] Look at : " << std::endl;
//	std::cout << "  * position : " << m_position << std::endl;
//	std::cout << "  * target : " << m_target << std::endl;
	m_matrix.LookAt(m_position, m_target);
	//m_matrix = m_matrix.Transpose();
}

void CameraAbstract::GetView()
{
	MatrixManagement::Instance().SetViewMatrix(m_matrix);
}

void CameraAbstract::SendInvMatrix()
{
	Math::CMatrix4 matrix;
	matrix.LookAt(m_position, m_target);
	matrix = matrix.Inverse();
	glShaderManager::Instance().currentShader()->setUniformMatrix4fv("InvViewMatrix", matrix);
}
