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
#include "MatrixManagement.h"
#include <Debug/Exceptions.h>

SINGLETON_IMPL(CMatrixManager)

CMatrixManager::CMatrixManager(int maxMatrix) :
m_maxMatrix(maxMatrix)
{
	m_identityMatrix.Identity();
}

CMatrixManager::~CMatrixManager()
{
	if(!m_matrix.empty())
		std::cout << "[WARNING] There still somes matrix in the stack..." << std::endl;
}

void CMatrixManager::PushMatrix(const Math::CMatrix4& matrix)
{
	// Debug limit
	if(m_maxMatrix < m_matrix.size())
		throw CException("Matrix stack is full.");

	// stack the new matrix
	if(m_matrix.empty())
		m_matrix.push_back(matrix);
	else
		m_matrix.push_back(m_matrix.back()*matrix);

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);

}

void CMatrixManager::PopMatrix()
{
	if(m_matrix.empty())
		throw CException("Matrix stack is empty. Unable to pop matrix");
	m_matrix.pop_back();

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);
}

bool CMatrixManager::IsEmpty() const
{
	return m_matrix.empty();
}

int CMatrixManager::StackSize() const
{
	return m_matrix.size();
}

const Math::CMatrix4& CMatrixManager::GetMatrix(MatrixType type)
{
	if(type == MODEL_MATRIX)
	{
		if(m_matrix.empty())
			return m_identityMatrix;
		return m_matrix.back();
	}
	else if(type == VIEW_MATRIX)
	{
		return m_viewMatrix;
	}
	else if(type == PROJECTION_MATRIX)
	{
		return m_projectionMatrix;
	}
	else if(type == NORMAL_MATRIX)
	{
		// Update
		m_normalMatrix = GetMatrix(MODEL_MATRIX).Inverse().Transpose();
		return m_normalMatrix;
	}
	else
	{
		throw CException("Unknow type");
	}
}

void CMatrixManager::SetProjectionMatrix(const Math::CMatrix4& matrix)
{
	m_projectionMatrix = matrix;
	m_signal_event.emit(PROJECTION_MATRIX);
}

void CMatrixManager::SetViewMatrix(const Math::CMatrix4& matrix)
{
	m_viewMatrix = matrix;
	m_signal_event.emit(VIEW_MATRIX);
}

sigc::signal<void, MatrixType>& CMatrixManager::GetSignalEvent()
{
	return m_signal_event;
}

