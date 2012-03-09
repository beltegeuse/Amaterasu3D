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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

//==========================================================
// En-tetes
//==========================================================
#include <System/Resource.h>
#include <Singleton.h>
#include <map>
#include <string>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Gestionnaire des ressources associees e un fichier
////////////////////////////////////////////////////////////
class CResourceManager: public CSingleton<ama3D::CResourceManager>
{
MAKE_SINGLETON(CResourceManager)

public:

	//----------------------------------------------------------
	// Recupere une ressource
	//----------------------------------------------------------
	template<class T> T* Get(const std::string& Name) const;

	//----------------------------------------------------------
	// Ajoute une ressource
	//----------------------------------------------------------
	void Add(const std::string& Name, IResource* Resource);

	//----------------------------------------------------------
	// Retire une ressource
	//----------------------------------------------------------
	void Remove(const std::string& Name);

private:

	//----------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------
	CResourceManager();

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	~CResourceManager();

	//----------------------------------------------------------
	// Types
	//----------------------------------------------------------
	typedef std::map<std::string, IResource*> TResourcesMap;

	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	TResourcesMap m_Resources; ///< Table contenant les ressources associees e leur nom de fichier
};

#include "ResourceManager.inl"
}

#endif // RESOURCEMANAGER_H
