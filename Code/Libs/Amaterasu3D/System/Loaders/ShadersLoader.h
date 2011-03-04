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

#ifndef SHADERSLOADER_H
#define SHADERSLOADER_H

//==========================================================
// En-têtes
//==========================================================
#include <System/Loaders/Loader.h>
#include <System/Resources/ShaderBase.h>

/////////////////////////////////////////////////////////////
/// Loader de shaders Cg
/////////////////////////////////////////////////////////////
class CShadersLoader : public ILoader<IShaderBase>
{
public :

	//----------------------------------------------------------
	// Construit le loader
	//----------------------------------------------------------
	CShadersLoader();

	//----------------------------------------------------------
	// Détruit le loader
	//----------------------------------------------------------
	~CShadersLoader();

	//----------------------------------------------------------
	// Charge un shader à partir d'un fichier
	//----------------------------------------------------------
	virtual IShaderBase* LoadFromFile(const std::string& Filename);
};


#endif // SHADERSLOADER_H
