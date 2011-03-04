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
// En-t�tes
//==========================================================
#include <System/Resources/Resource.h>
#include <System/ResourceManager.h>


/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
////////////////////////////////////////////////////////////
IResource::IResource() :
m_Name    (""),
m_RefCount(1)
{

}


/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
IResource::~IResource()
{
    if (m_Name != "")
        CResourceManager::Instance()->Remove(m_Name);
}


/////////////////////////////////////////////////////////////
/// Renvoie le nom associ� � la ressource
///
/// \return Nom attribu� � la ressource
///
////////////////////////////////////////////////////////////
const std::string& IResource::GetName() const
{
    return m_Name;
}


/////////////////////////////////////////////////////////////
/// Ajoute une r�f�rence sur la ressource
///
////////////////////////////////////////////////////////////
void IResource::AddRef()
{
    ++m_RefCount;
}


/////////////////////////////////////////////////////////////
/// Retire une r�f�rence sur la ressource
///
/// \return Nombre de r�f�rences restantes sur la ressource
///
////////////////////////////////////////////////////////////
int IResource::Release()
{
    // D�cr�mentation du compteur de r�f�rences
    int RefCount = --m_RefCount;

    // S'il n'y a plus de r�f�rence sur la ressource, on la d�truit
    if (RefCount == 0)
        delete this;

    return RefCount;
}
