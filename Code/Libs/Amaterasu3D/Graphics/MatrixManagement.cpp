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
#include "MatrixManagement.h"
#include <Debug/Exceptions.h>
#include <System/SettingsManager.h>
SINGLETON_IMPL(CMatrixManager)

CMatrixManager::CMatrixManager(int maxMatrix) :
		m_MaxMatrix(maxMatrix), m_MatrixMode(MATRIX_3D)
{
	m_IdentityMatrix.Identity();
}

CMatrixManager::~CMatrixManager()
{
	if (!m_Matrix.empty())
		std::cout << "[WARNING] There still somes matrix in the stack..."
				<< std::endl;
}

void CMatrixManager::PushMatrix(const Math::CMatrix4& matrix)
{
	// Debug limit
	if (m_MaxMatrix < (int) m_Matrix.size())
		throw CException("Matrix stack is full.");

	// stack the new matrix
	if (m_Matrix.empty())
		m_Matrix.push_back(matrix);
	else
		m_Matrix.push_back(m_Matrix.back() * matrix);

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);

}

void CMatrixManager::PopMatrix()
{
	if (m_Matrix.empty())
		throw CException("Matrix stack is empty. Unable to pop matrix");
	m_Matrix.pop_back();

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);
}

bool CMatrixManager::IsEmpty() const
{
	return m_Matrix.empty();
}

int CMatrixManager::StackSize() const
{
	return m_Matrix.size();
}

const Math::CMatrix4& CMatrixManager::GetMatrix(MatrixType type)
{
	if (type == MODEL_MATRIX)
	{
		if (m_Matrix.empty())
			return m_IdentityMatrix;
		return m_Matrix.back();
	}
	else if (type == VIEW_MATRIX)
	{
		return m_ViewMatrix;
	}
	else if (type == PROJECTION_MATRIX)
	{
		return m_ProjectionMatrix;
	}
	else if (type == NORMAL_MATRIX)
	{
		// Update
		m_NormalMatrix = GetMatrix(MODEL_MATRIX).Inverse().Transpose();
		return m_NormalMatrix;
	}
	else
	{
		throw CException("Unknow type");
	}
}

void CMatrixManager::SetProjectionMatrix(const Math::CMatrix4& matrix)
{
	m_ProjectionMatrix = matrix;
	m_signal_event.emit(PROJECTION_MATRIX);
}

void CMatrixManager::SetViewMatrix(const Math::CMatrix4& matrix)
{
	m_ViewMatrix = matrix;
	m_signal_event.emit(VIEW_MATRIX);
}

void CMatrixManager::SetModeMatrix(MatrixMode mode)
{
	// Nothings to Do
	if (m_MatrixMode == mode)
		return;

	if (mode == MATRIX_2D)
	{
		// Compute the Matrix
		Math::CMatrix4 mat;
		Math::TVector2I size =
				CSettingsManager::Instance().GetSizeRenderingWindow();
		mat.OrthoOffCenter(0, 0, size.x, size.y);
		m_ProjectionMatrixOld = m_ProjectionMatrix;
		SetProjectionMatrix(mat);
		glDisable(GL_DEPTH_TEST);
	}
	else if (mode == MATRIX_3D)
	{
		// Revert good perpective matrix
		SetProjectionMatrix(m_ProjectionMatrixOld);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		throw CException("Unknown matrix mode");
	}

	m_MatrixMode = mode;
}

sigc::signal<void, MatrixType>& CMatrixManager::GetSignalEvent()
{
	return m_signal_event;
}

