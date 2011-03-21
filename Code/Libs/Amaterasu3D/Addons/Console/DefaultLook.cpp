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
// En-têtes
//==========================================================
#include "DefaultLook.h"
#include <sstream>
#include <Addons/Console/Functor.h>
#include <Addons/Console/Console.h>

#include <System/SettingsManager.h>

////////////////////////////////////////////////////////////
// Données statiques
////////////////////////////////////////////////////////////
const std::string DefaultLook::s_Fonts[] =  {"arial"};


////////////////////////////////////////////////////////////
/// Constructeur par défaut
///
////////////////////////////////////////////////////////////
DefaultLook::DefaultLook() :
m_State      (STOPPED),
m_CurrentFont(0),
m_ShowText(false)
{

	m_BackgroundTexture = Texture::LoadFromFile("ConsoleBG.tga");

    // Position initiale : invisible
    m_Transfo.SetScaling(0, 0, 0);

    // Création de la première ligne
    AddLine();

    // Enregistrement des commandes spéciales console
    CConsole::Instance().RegisterCommand("clear", Console::Bind(&std::list<CGraphicString>::clear, m_Lines));
    CConsole::Instance().RegisterCommand("font",  Console::Bind(&DefaultLook::NextFont, *this));

    m_Height = 1.0 - (210.0 / SettingsManager::Instance().GetSizeRenderingWindow().y)*2;
}


////////////////////////////////////////////////////////////
/// Fonction appelée lors de la mise à jour de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Update()
{
    static float Scale = 0.0f;

    // Mise à jour de la position de la console selon son état courant
    // et mise à jour de celui-ci
    if (m_State == SHOWING)
    {
        m_Transfo.SetScaling(Scale, Scale, 1);
        Scale += 0.01f;

        if (Scale > 1.0f)
        {
            m_Transfo.SetScaling(1, 1, 1);
            m_State = STOPPED;
            Scale   = 1.0f;
            m_ShowText = true;
        }
    }
    else if (m_State == HIDDING)
    {
    	m_ShowText = false;
        m_Transfo.SetScaling(Scale, Scale, 1);
        Scale -= 0.01f;

        if (Scale < 0.01f)
        {
            m_Transfo.SetScaling(0, 0, 0);
            m_State = STOPPED;
            Scale   = 0.0f;
        }
    }
}


////////////////////////////////////////////////////////////
/// Fonction appelée lors de l'affichage de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Draw()
{

    // Envoi de la matrice de transformation de la console
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const float*)m_Transfo);

	m_BackgroundTexture->activateTextureMapping();
	m_BackgroundTexture->activateTexture();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1.0, m_Height);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(1.0, m_Height);
	glEnd();

    m_BackgroundTexture->desactivateTextureMapping();

    if(m_ShowText)
    {
		// Affichage des lignes de texte
		for (std::list<CGraphicString>::iterator i = m_Lines.begin(); i != m_Lines.end(); ++i)
			i->Draw();
    }

    // Restauration de la matrice de vue précédente
    //Renderer.PopMatrix(MAT_MODELVIEW);
}


////////////////////////////////////////////////////////////
/// Fonction appelée lors de l'activation / désactivation de la console
///
/// \param Visible : Nouvel état de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Show(bool Visible)
{
    m_State = Visible ? SHOWING : HIDDING;
}


////////////////////////////////////////////////////////////
/// Fonction appelée après l'appel à une commande
///
/// \param Result : Résultat de l'appel de la commande
///
////////////////////////////////////////////////////////////
void DefaultLook::CommandCalled(const std::string& Result)
{
    if (!Result.empty())
        AddLine(Result, CColor(150, 200, 150));
    AddLine();
}


////////////////////////////////////////////////////////////
/// Fonction appelée à chaque changement de la ligne courante
///
/// \param NewText : Nouveau texte
///
////////////////////////////////////////////////////////////
void DefaultLook::TextChanged(const std::string& NewText)
{
    m_Lines.front().Text = "> " + NewText;
}


////////////////////////////////////////////////////////////
/// Fonction appelée en cas d'erreur
///
/// \param Message : Message décrivant l'erreur
///
////////////////////////////////////////////////////////////
void DefaultLook::Error(const std::string& Message)
{
    // Décomposition du message en lignes
    std::string Line;
    std::istringstream iss(Message);
    while (std::getline(iss, Line))
        AddLine(Line, CColor(200, 150, 150));

    AddLine();
}


////////////////////////////////////////////////////////////
/// Ajoute une ligne
///
/// \param Line :  Texte de la ligne à ajouter
/// \param Color : Couleur du texte
///
////////////////////////////////////////////////////////////
void DefaultLook::AddLine(const std::string& Line, const CColor& Color)
{
    // On supprime la dernière ligne si nécessaire
    if (m_Lines.size() == 10)
        m_Lines.pop_back();

    // On décale les autres
    for (std::list<CGraphicString>::iterator i = m_Lines.begin(); i != m_Lines.end(); ++i)
    {
        i->Position.y -= 20;
        i->Color.Set(i->Color.GetRed(), i->Color.GetGreen(), i->Color.GetBlue(), i->Color.GetAlpha() - 20);
    }

    // Et on crée la nouvelle
    //TODO : Add Color
    m_Lines.push_front(CGraphicString(Math::TVector2F(10, 190), Line, s_Fonts[m_CurrentFont], 20));
}


////////////////////////////////////////////////////////////
/// Sélectionne la police suivante
///
/// \return Nom de la nouvelle police
///
////////////////////////////////////////////////////////////
std::string DefaultLook::NextFont()
{
    m_CurrentFont = (m_CurrentFont + 1) % (sizeof(s_Fonts) / sizeof(*s_Fonts));

    return "Nouvelle police : \"" + s_Fonts[m_CurrentFont] + "\"";
}
