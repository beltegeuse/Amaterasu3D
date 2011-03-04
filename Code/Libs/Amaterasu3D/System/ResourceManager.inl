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


/////////////////////////////////////////////////////////////
/// Renvoie un pointeur sur une ressource déjà chargée
///
/// \param Name : Nom de la ressource à récupérer
///
/// \return Pointeur sur la ressource, NULL si elle n'a pas été chargée
///
////////////////////////////////////////////////////////////
template <class T>
inline T* CResourceManager::Get(const std::string& Name) const
{
    // Recherche de la ressource
    TResourcesMap::const_iterator It = m_Resources.find(Name);

    // Si on l'a trouvé on la renvoie, sinon on renvoie NULL
    if (It != m_Resources.end())
    {
        It->second->AddRef();
        return static_cast<T*>(It->second);
    }
    else
    {
        return NULL;
    }
}
