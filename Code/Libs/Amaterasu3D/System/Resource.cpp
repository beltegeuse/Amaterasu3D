//==========================================================
// Yes::Engine - Free C++ 3D engine
//
// Copyright (C) 2004-2005 Laurent Gomila
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
// E-mail : laurent.gom@gmail.com
//==========================================================

//==========================================================
// En-tetes
//==========================================================
#include <System/Resource.h>
#include <System/ResourceManager.h>
namespace ama3D
{
/////////////////////////////////////////////////////////////
/// Constructeur par defaut
///
////////////////////////////////////////////////////////////
IResource::IResource() :
		m_Name(""), m_RefCount(1)
{

}

/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
IResource::~IResource()
{
	if (m_Name != "")
		CResourceManager::Instance().Remove(m_Name);
}

/////////////////////////////////////////////////////////////
/// Renvoie le nom associe e la ressource
///
/// \return Nom attribue e la ressource
///
////////////////////////////////////////////////////////////
const std::string& IResource::GetName() const
{
	return m_Name;
}

/////////////////////////////////////////////////////////////
/// Ajoute une reference sur la ressource
///
////////////////////////////////////////////////////////////
void IResource::AddRef()
{
	++m_RefCount;
}

/////////////////////////////////////////////////////////////
/// Retire une reference sur la ressource
///
/// \return Nombre de references restantes sur la ressource
///
////////////////////////////////////////////////////////////
int IResource::Release()
{
	// Decrementation du compteur de references
	int RefCount = --m_RefCount;

	// S'il n'y a plus de reference sur la ressource, on la detruit
	if (RefCount == 0)
		delete this;

	return RefCount;
}

}
