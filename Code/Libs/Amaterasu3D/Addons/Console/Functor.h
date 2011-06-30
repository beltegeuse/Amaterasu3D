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

#ifndef FUNCTOR_H
#define FUNCTOR_H

//==========================================================
// En-t�tes
//==========================================================
#include <Utilities/SmartPtr.h>
#include <Utilities/StringUtils.h>
#include <Debug/Exceptions.h>
#include <string>

namespace Console
{
////////////////////////////////////////////////////////////
/// Classe de base pour les classes encapsulant des fonctions
////////////////////////////////////////////////////////////
class IFunction
{
public:

	//----------------------------------------------------------
	// Destructeur virtuel
	//----------------------------------------------------------
	virtual ~IFunction()
	{
	}

	//----------------------------------------------------------
	// Effectue l'appel de fonction
	//----------------------------------------------------------
	virtual std::string Execute(const std::string& Params) = 0;
};

////////////////////////////////////////////////////////////
/// Classe d�finissant un foncteur (objet fonction)
////////////////////////////////////////////////////////////
class CFunctor
{
public:

	//----------------------------------------------------------
	// Construit le foncteur � partir d'une fonction
	//----------------------------------------------------------
	CFunctor(IFunction* Func = NULL);

	//----------------------------------------------------------
	// Effectue l'appel � la fonction
	//----------------------------------------------------------
	std::string operator ()(const std::string& Params = "") const;

private:

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	CSmartPtr<IFunction> m_Function; ///< Pointeur sur l'objet stockant la fonction
};

#include "Functor.inl"

} // namespace Console

#endif // FUNCTOR_H
