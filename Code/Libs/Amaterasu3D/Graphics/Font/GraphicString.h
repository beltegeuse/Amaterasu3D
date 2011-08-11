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

#ifndef GRAPHICSTRING_H
#define GRAPHICSTRING_H

//==========================================================
// En-tetes
//==========================================================
#include <Math/Rectangle.h>
#include <Graphics/Color.h>
#include <Singleton.h>
#include <Math/Vector2.h>

#include <map>
#include <string>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Chaine de caractere graphique (affichable a l ecran)
////////////////////////////////////////////////////////////
class CGraphicString
{
public:

	//----------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------
	CGraphicString(const Math::TVector2F& StringPosition = Math::TVector2F(0, 0),
			const std::string& StringText = "Seagull", const std::string& fontAlias = "Cheeseburger", int size = 18);
	virtual ~CGraphicString();
	//----------------------------------------------------------
	// Affiche la chaine de caracteres a l'ecran
	//----------------------------------------------------------
	virtual void Draw();

	// Permet de changer la chaine de caractere
	virtual void SetString(std::string text);

	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	Math::TVector2F Position; ///< Position du texte a l'ecran
	std::string Text; ///< Chaine de caracteres
	std::string FontAlias;
	CColor Color; ///< Couleur du texte
	int Size; ///< Character size
};
}

#endif // GRAPHICSTRING_H
