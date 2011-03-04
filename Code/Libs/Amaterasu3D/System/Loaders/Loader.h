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

#ifndef LOADER_H
#define LOADER_H

//==========================================================
// En-têtes
//==========================================================
//#include <Config/Fwd.h>
#include <Debug/Exceptions.h>
#include <string>

////////////////////////////////////////////////////////////
// Classe de base pour les importateurs / exportateurs de medias
////////////////////////////////////////////////////////////
template <class T>
class ILoader
{
public :

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~ILoader() {}

	//----------------------------------------------------------
	// Charge un T à partir d'un fichier
	//----------------------------------------------------------
	virtual T* LoadFromFile(const std::string& Filename)
	{
		throw CLoadingFailed(Filename, "Le loader enregistré pour ce format ne prend pas en charge l'importation");
	}

	//----------------------------------------------------------
	// Enregistre un T dans un fichier
	//----------------------------------------------------------
	virtual void SaveToFile(const T* Object, const std::string& Filename)
	{
		throw CLoadingFailed(Filename, "Le loader enregistré pour ce format ne prend pas en charge l'exportation");
	}

protected :

	//----------------------------------------------------------
	// Constructeur - protégé : rend la classe abstraite
	//----------------------------------------------------------
	ILoader() {}
};


#endif // LOADER_H
