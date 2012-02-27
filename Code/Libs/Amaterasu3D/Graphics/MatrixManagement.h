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
#ifndef MATRIXMANAGEMENT_H_
#define MATRIXMANAGEMENT_H_
#include <Enum.h>
#include "glm/glm.hpp"
#include <vector>
#include <Singleton.h>
#include <boost/signal.hpp>

namespace ama3D
{
class CMatrixManager: public CSingleton<CMatrixManager>
{
MAKE_SINGLETON(CMatrixManager)
private:
	/*
	 * Attributes
	 */
	// World Matrix
	int m_MaxMatrix; ///< Max world matrix
	std::vector<glm::mat4x4> m_Matrix; ///< World matrix stack
	glm::mat4x4 m_IdentityMatrix; ///< Matrix when the stack is empty
	// Other matrix
	glm::mat4x4 m_ProjectionMatrix; ///< Projection matrix
	glm::mat4x4 m_ProjectionMatrixOld; ///< To save the matrix
	glm::mat4x4 m_ViewMatrix; ///< View matrix
	glm::mat4x4 m_NormalMatrix; ///< Normal matrix
	// Others attributes
	MatrixMode m_MatrixMode; ///< To know the matrix mode
	boost::signal<void (MatrixType)> m_signal_event; ///< To update all shaders Matrix attributes

	/*
	 * Constructors & Destructors
	 */
	// Singleton implements
	CMatrixManager(int maxMatrix = 50);
	// Destructor ...
	virtual ~CMatrixManager();
public:

	/**
	 * ModelView Matrix
	 */
	// Public methods
	void PushMatrix(const glm::mat4x4& matrix);
	void PopMatrix();
	// States of the stack
	bool IsEmpty() const;
	int StackSize() const;
	// Get Matrix
	const glm::mat4x4& GetMatrix(MatrixType type);
	/**
	 * Projection matrix
	 */
	void SetProjectionMatrix(const glm::mat4x4& matrix);

	/**
	 * View matrix
	 */
	void SetViewMatrix(const glm::mat4x4& matrix);

	/**
	 * Matrix Mode
	 */
	void SetModeMatrix(MatrixMode mode);

	//** to subscribe to the signal
	boost::signal<void (MatrixType)>& GetSignalEvent();
};
}

#endif /* MATRIXMANAGEMENT_H_ */
