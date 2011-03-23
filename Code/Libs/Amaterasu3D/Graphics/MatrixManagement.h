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
#ifndef MATRIXMANAGEMENT_H_
#define MATRIXMANAGEMENT_H_
#include <Enum.h>
#include <Math/Matrix4.h>
#include <vector>
#include <Singleton.h>
#include <sigc++/signal.h>

class MatrixManagement : public CSingleton<MatrixManagement>
{
	MAKE_SINGLETON(MatrixManagement);
private:
	// Attributs
	int m_maxMatrix;
	std::vector<Math::CMatrix4> m_matrix;
	Math::CMatrix4 m_projectionMatrix;
	Math::CMatrix4 m_viewMatrix;
	Math::CMatrix4 m_identityMatrix;
	Math::CMatrix4 m_normalMatrix;

	sigc::signal<void, MatrixType> m_signal_event;

	// Singleton implements
	MatrixManagement(int maxMatrix = 50);
	// Destructor ...
	virtual ~MatrixManagement();
public:

	/**
	 * ModelView Matrix
	 */
	// Public methods
	void PushMatrix(const Math::CMatrix4& matrix);
	void PopMatrix();
	// States of the stack
	bool IsEmpty() const;
	int StackSize() const;
	// Get Matrix
	const Math::CMatrix4& GetMatrix(MatrixType type);
	/**
	 * Projection matrix
	 */
	void SetProjectionMatrix(const Math::CMatrix4& matrix);

	/**
	 * View matrix
	 */
	void SetViewMatrix(const Math::CMatrix4& matrix);

	//** to subscribe to the signal
	sigc::signal<void, MatrixType>& GetSignalEvent();
};

#endif /* MATRIXMANAGEMENT_H_ */
