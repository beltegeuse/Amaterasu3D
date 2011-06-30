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
#include <Addons/Console/Console.h>
#include <Debug/Exceptions.h>
#include <sstream>
#include <Addons/Console/DefaultLook.h>

////////////////////////////////////////////////////////////
// Impl�mentation des m�thodes du singleton
////////////////////////////////////////////////////////////
SINGLETON_IMPL(CConsole)


////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
////////////////////////////////////////////////////////////
CConsole::CConsole() :
m_Current(""),
m_Look   (NULL),
m_Enabled(false)
{
    RegisterCommand("?", Console::Bind(&CConsole::GetCommands, *this));
}

CConsole::~CConsole()
{
}


////////////////////////////////////////////////////////////
/// Change l'apparence de la console
///
/// \param NewLook : Pointeur sur le nouveau look
///
////////////////////////////////////////////////////////////
void CConsole::ChangeLook(Console::ILook* NewLook)
{
    Assert(NewLook != NULL);

    m_Look = NewLook;
}

////////////////////////////////////////////////////////////
/// Enregistre une nouvelle commande
///
/// \param Name :     Nom de la commande
/// \param Function : Foncteur contenant la fonction associ�e
///
////////////////////////////////////////////////////////////
void CConsole::RegisterCommand(const std::string& Name, const Console::CFunctor& Function)
{
    m_Commands[Name] = Function;
}


////////////////////////////////////////////////////////////
/// Envoie un nouveau caract�re � la console
///
/// \param Character : Caract�re
///
////////////////////////////////////////////////////////////
void CConsole::SendChar(char Character)
{
    // Si la console n'est pas active on ne traite pas le caract�re
    if (!m_Enabled)
        return;

    // Traitement du caract�re
    switch (Character)
    {
        // Saut de ligne : on traite la commande et on efface la ligne
        case '\n' :
        case '\r' :
            if (!m_Current.empty())
            {
                ProcessCurrent();
                m_Current.clear();
            }
            break;

        // Backspace : on efface le dernier caract�re
        case '\b' :
            if (!m_Current.empty())
                m_Current.erase(m_Current.size() - 1);
            break;

        // Tout le reste : on ajoute le caract�re � la ligne courante
        default :
            m_Current += Character;
            break;
    }

    // On notifie au "look" que le texte courant vient de changer
    m_Look->TextChanged(m_Current);
}


////////////////////////////////////////////////////////////
/// Met � jour la console
///
////////////////////////////////////////////////////////////
void CConsole::FrameStarted(double delta)
{
    m_Look->Update(delta);
}


////////////////////////////////////////////////////////////
/// Affiche la console
///
////////////////////////////////////////////////////////////
void CConsole::Draw() const
{
    m_Look->Draw();
}


////////////////////////////////////////////////////////////
/// Active ou d�sactive la console
///
/// \param Enabled : Active / inactive
///
////////////////////////////////////////////////////////////
void CConsole::Enable(bool Enabled)
{
    m_Enabled = Enabled;
    m_Look->Show(Enabled);
}

bool CConsole::IsEnable() const
{
	return m_Enabled;
}


////////////////////////////////////////////////////////////
/// Donne la liste des commandes enregistr�es
///
/// \return Cha�ne contenant la liste des commandes enregistr�es
///
////////////////////////////////////////////////////////////
std::string CConsole::GetCommands() const
{
    std::string List;
    for (TCommandTable::const_iterator i = m_Commands.begin(); i != m_Commands.end(); ++i)
        List += i->first + " ";

    return List;
}


////////////////////////////////////////////////////////////
/// Traite la ligne courante et appelle la fonction correspondante
///
////////////////////////////////////////////////////////////
void CConsole::ProcessCurrent()
{
    // On r�cup�re la commande
    std::string Command;
    std::istringstream iss(m_Current);
    iss >> Command;

    // On recherche la commande dans la table des commandes
    TCommandTable::iterator It = m_Commands.find(Command);

    // Si elle s'y trouve on appelle la fonction correspondante, sinon on g�n�re une erreur
    if (It != m_Commands.end())
    {
        // R�cup�ration des param�tres
        std::string Params;
        std::getline(iss, Params);

        // Appel du foncteur correspondant � la commande -
        // s'il y a une erreur on la rattrape et on l'affiche dans la console
        try
        {
            m_Look->CommandCalled(It->second(Params));
        }
        catch (std::exception& E)
        {
            m_Look->Error(E.what());
        }
    }
    else
    {
        m_Look->Error("Commande \"" + Command + "\" inconnue");
    }
}
