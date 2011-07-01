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

#ifndef LOOK_H
#define LOOK_H

//==========================================================
// En-t�tes
//==========================================================
#include <string>

namespace ama3D
{
namespace Console
{
////////////////////////////////////////////////////////////
/// Classe � d�river pour personnaliser l'apparence de la console
////////////////////////////////////////////////////////////
class ILook
{
public:

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~ILook()
	{
	}

	//----------------------------------------------------------
	// Fonction appel�e lors de la mise � jour de la console (Listener functions)
	//----------------------------------------------------------
	virtual void Update(double delta) = 0;

	//----------------------------------------------------------
	// Fonction appel�e lors de l'affichage de la console
	//----------------------------------------------------------
	virtual void Draw() = 0; // FIXME add CONST

	//----------------------------------------------------------
	// Fonction appel�e lors de l'activation / d�sactivation de la console
	//----------------------------------------------------------
	virtual void Show(bool Visible) = 0;

	//----------------------------------------------------------
	// Fonction appel�e apr�s l'appel � une commande
	//----------------------------------------------------------
	virtual void CommandCalled(const std::string& Result) = 0;

	//----------------------------------------------------------
	// Fonction appel�e � chaque changement de la ligne courante
	//----------------------------------------------------------
	virtual void TextChanged(const std::string& NewText) = 0;

	//----------------------------------------------------------
	// Fonction appel�e en cas d'erreur
	//----------------------------------------------------------
	virtual void Error(const std::string& Message) = 0;
};

} // namespace Console
}

#endif // LOOK_H
