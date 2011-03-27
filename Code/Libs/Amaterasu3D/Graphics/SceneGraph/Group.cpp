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
#include <Graphics/SceneGraph/Group.h>
#include <Logger/Logger.h>
#include <Graphics/MatrixManagement.h>
namespace SceneGraph
{
/** \brief Constructeur d'un noeud group */
Group::Group()
{
}
Group::~Group()
{
	for(unsigned int cpt=0, size=(unsigned int)m_objects.size() ; cpt<size ; ++cpt)
	{
		delete m_objects[cpt] ;
	}
}
/** \brief Recuperation des fils */
const std::vector<DrawObject*> & Group::GetChilds() const
{ return m_objects ; }
/** \brief Ajout d'un fils au groupe d'objets. */
void Group::AddChild(DrawObject * object)
{
	m_objects.push_back(object) ;
}
/** \brief Gestion de l'affichage OpenGL */
void Group::Draw()
{
	CMatrixManager::Instance().PushMatrix(m_matrix_transform);
	for(unsigned int cpt=0 ; cpt<m_objects.size() ; cpt++)
	{
		m_objects[cpt]->Draw();
	}
	CMatrixManager::Instance().PopMatrix();
}

void Group::ClearAllChilds()
{
	for(unsigned int cpt=0, size=(unsigned int)m_objects.size() ; cpt<size ; ++cpt)
	{
		delete m_objects[cpt] ;
	}

	m_objects.erase(m_objects.begin(), m_objects.end());
}

void Group::LoadTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = matrix;
}
void Group::MultTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = m_matrix_transform*matrix;
}
const Math::CMatrix4& Group::GetTransformMatrix() const
{
	return m_matrix_transform;
}

} // namespace

