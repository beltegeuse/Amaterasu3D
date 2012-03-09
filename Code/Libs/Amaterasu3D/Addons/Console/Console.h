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

#ifndef CONSOLE_H
#define CONSOLE_H

//==========================================================
// En-tetes
//==========================================================
#include <Singleton.h>
#include <Addons/Console/Functor.h>
#include <Addons/Console/Look.h>
#include <map>

#include <System/EventListeners.h>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Gestion de la console
////////////////////////////////////////////////////////////
class CConsole: public CSingleton<ama3D::CConsole>, public FrameListener
{
MAKE_SINGLETON(ama3D::CConsole)

public:

	//----------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------
	CConsole();
	virtual ~CConsole();
	//----------------------------------------------------------
	// Change l'apparence de la console
	//----------------------------------------------------------
	void ChangeLook(Console::ILook* NewLook);

	//----------------------------------------------------------
	// Enregistre une nouvelle commande
	//----------------------------------------------------------
	void RegisterCommand(const std::string& Name,
			const Console::CFunctor& Function);

	//----------------------------------------------------------
	// Envoie un nouveau caractere e la console
	//----------------------------------------------------------
	void SendChar(char Character);

	//----------------------------------------------------------
	// Met e jour la console
	//----------------------------------------------------------
	virtual void FrameStarted(double delta);
	virtual void FrameEnded()
	{
	} ///< Doesn't need

	//----------------------------------------------------------
	// Affiche la console
	//----------------------------------------------------------
	void Draw() const;

	//----------------------------------------------------------
	// Active ou desactive la console
	//----------------------------------------------------------
	void Enable(bool Enabled);
	bool IsEnable() const;
private:

	//----------------------------------------------------------
	// Donne la liste des commandes enregistrees
	//----------------------------------------------------------
	std::string GetCommands() const;

	//----------------------------------------------------------
	// Traite la ligne courante et appelle la fonction correspondante
	//----------------------------------------------------------
	void ProcessCurrent();

	//----------------------------------------------------------
	// Types
	//----------------------------------------------------------
	typedef std::map<std::string, Console::CFunctor> TCommandTable;

	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	TCommandTable m_Commands; ///< Table des commandes enregistrees
	std::string m_Current; ///< Ligne courante
	CSmartPtr<Console::ILook> m_Look; ///< Pointeur sur la classe gerant l'apparence de la console
	bool m_Enabled; ///< Indique si la console est active ou non
};
}
#endif // CONSOLE_H
