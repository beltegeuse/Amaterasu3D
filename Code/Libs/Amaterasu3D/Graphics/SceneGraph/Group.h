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
#ifndef _Group_H
#define _Group_H

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <Math/Matrix4.h>
#include <vector>

namespace SceneGraph
{

	/** \brief Noeud representant un groupe d'objets */
	class Group : public DrawObject
	{
	private:
		std::vector<DrawObject*> m_objects ;
		Math::CMatrix4 m_matrix_transform;

	public:
		/** \brief Constructeur d'un noeud group */
		Group();
		virtual ~Group();
		/** \brief Recuperation des fils */
		const std::vector<DrawObject*> & GetChilds() const;
		/** \brief Ajout d'un fils au groupe d'objets. */
		virtual void AddChild(DrawObject * object);
		/** \brief Gestion de l'affichage OpenGL */
		virtual void Draw();
		/**
		 * All transform matrix management
		 */
		void LoadTransformMatrix(const Math::CMatrix4& matrix);
		void MultTransformMatrix(const Math::CMatrix4& matrix);
		const Math::CMatrix4& GetTransformMatrix() const;

	} ;

}

#endif
