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

#ifndef RESOURCE_H
#define RESOURCE_H

//==========================================================
// En-têtes
//==========================================================
#include <string>


////////////////////////////////////////////////////////////
/// Classe de base pour les ressources
////////////////////////////////////////////////////////////
class IResource
{
public :

	//----------------------------------------------------------
	// Constructeur par défaut
	//----------------------------------------------------------
	IResource();

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~IResource() = 0;

	//----------------------------------------------------------
	// Renvoie le nom associé à la ressource
	//----------------------------------------------------------
	const std::string& GetName() const;

	//----------------------------------------------------------
	// Ajoute une référence sur la ressource
	//----------------------------------------------------------
	void AddRef();

	//----------------------------------------------------------
	// Retire une référence sur la ressource
	//----------------------------------------------------------
	int Release();

private :

	//----------------------------------------------------------
	// Amis
	//----------------------------------------------------------
	friend class CResourceManager;

	//----------------------------------------------------------
	// - Copie interdite -
	//----------------------------------------------------------
	IResource(IResource&);
	void operator =(IResource&);

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	std::string m_Name;     ///< Nom associé à la ressource
	int         m_RefCount; ///< Compteur de références
};



#endif // RESOURCE_H
