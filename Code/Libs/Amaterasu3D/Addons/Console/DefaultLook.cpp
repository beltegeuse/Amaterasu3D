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
#include "DefaultLook.h"
#include <sstream>
#include <Addons/Console/Functor.h>
#include <Addons/Console/Console.h>

#include <Logger/Logger.h>
#include <System/SettingsManager.h>
#include <Graphics/MatrixManagement.h>

namespace ama3D
{
////////////////////////////////////////////////////////////
// Donn�es statiques
////////////////////////////////////////////////////////////
const std::string DefaultLook::s_Fonts[] =
{ "Cheeseburger" };

////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
////////////////////////////////////////////////////////////
DefaultLook::DefaultLook() :
		m_State(STOPPED), m_CurrentFont(0), m_Rectangle(0), m_ShowText(false)
{

	m_BackgroundTexture = Texture::LoadFromFile("ConsoleBG.tga");

	// Position initiale : invisible
	m_Transfo.SetScaling(0, 0, 0);

	// Cr�ation de la premi�re ligne
	AddLine(); // FIXME

	// Enregistrement des commandes sp�ciales console
	CConsole::Instance().RegisterCommand("clear",
			Console::Bind(&std::list<CGraphicString>::clear, m_Lines));
	CConsole::Instance().RegisterCommand("font",
			Console::Bind(&DefaultLook::NextFont, *this));

	m_Height = 1.0
			- (210.0 / CSettingsManager::Instance().GetSizeRenderingWindow().y)
					* 2;
}

DefaultLook::~DefaultLook()
{
	if (m_Rectangle != 0)
	{
		delete m_Rectangle;
	}
}

////////////////////////////////////////////////////////////
/// Fonction appel�e lors de la mise � jour de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Update(double delta)
{
	static float Scale = 0.0f;

	// Mise � jour de la position de la console selon son �tat courant
	// et mise � jour de celui-ci
	if (m_State == SHOWING)
	{
		m_Transfo.SetScaling(Scale, Scale, 1);
		Scale += delta * 4.0;

		if (Scale > 1.0f)
		{
			m_Transfo.SetScaling(1, 1, 1);
			m_State = STOPPED;
			Scale = 1.0f;
			m_ShowText = true;
		}
	}
	else if (m_State == HIDDING)
	{
		m_ShowText = false;
		m_Transfo.SetScaling(Scale, Scale, 1);
		Scale -= delta * 4.0;

		if (Scale < 0.01f)
		{
			m_Transfo.SetScaling(0, 0, 0);
			m_State = STOPPED;
			Scale = 0.0f;
		}
	}
}

////////////////////////////////////////////////////////////
/// Fonction appel�e lors de l'affichage de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Draw()
{
	// On initialise au premier appel
	if (m_Rectangle == 0)
	{

		m_Rectangle = new Rectangle2D(
				Math::TVector2I(0, 0),
				Math::TVector2I(
						CSettingsManager::Instance().GetSizeRenderingWindow().x,
						210.0));
		m_Rectangle->GetObject().AddTextureMap(DIFFUSE_TEXTURE,
				m_BackgroundTexture);
		m_2DShader = CShaderManager::Instance().LoadShader("2DDraw.shader");
	}

	m_2DShader->Begin();
	// Envoi de la matrice de transformation de la console
	CMatrixManager::Instance().PushMatrix(m_Transfo);
	m_Rectangle->Render();

	m_2DShader->End(); ///FIXME: Normally add for texts

	if (m_ShowText)
	{
		// Affichage des lignes de texte
		for (std::list<CGraphicString>::iterator i = m_Lines.begin();
				i != m_Lines.end(); ++i)
			i->Draw();
	}

	CMatrixManager::Instance().PopMatrix();
}

////////////////////////////////////////////////////////////
/// Fonction appel�e lors de l'activation / d�sactivation de la console
///
/// \param Visible : Nouvel �tat de la console
///
////////////////////////////////////////////////////////////
void DefaultLook::Show(bool Visible)
{
	m_State = Visible ? SHOWING : HIDDING;
}

////////////////////////////////////////////////////////////
/// Fonction appel�e apr�s l'appel � une commande
///
/// \param Result : R�sultat de l'appel de la commande
///
////////////////////////////////////////////////////////////
void DefaultLook::CommandCalled(const std::string& Result)
{
	if (!Result.empty())
		AddLine(Result, CColor(150, 200, 150));
	AddLine();
}

////////////////////////////////////////////////////////////
/// Fonction appel�e � chaque changement de la ligne courante
///
/// \param NewText : Nouveau texte
///
////////////////////////////////////////////////////////////
void DefaultLook::TextChanged(const std::string& NewText)
{
	m_Lines.front().Text = "> " + NewText;
}

////////////////////////////////////////////////////////////
/// Fonction appel�e en cas d'erreur
///
/// \param Message : Message d�crivant l'erreur
///
////////////////////////////////////////////////////////////
void DefaultLook::Error(const std::string& Message)
{
	// D�composition du message en lignes
	std::string Line;
	std::istringstream iss(Message);
	while (std::getline(iss, Line))
		AddLine(Line, CColor(200, 150, 150));

	AddLine();
}

////////////////////////////////////////////////////////////
/// Ajoute une ligne
///
/// \param Line :  Texte de la ligne � ajouter
/// \param Color : Couleur du texte
///
////////////////////////////////////////////////////////////
void DefaultLook::AddLine(const std::string& Line, const CColor& Color)
{
	// On supprime la derni�re ligne si n�cessaire
	if (m_Lines.size() == 10)
		m_Lines.pop_back();

	// On d�cale les autres
	for (std::list<CGraphicString>::iterator i = m_Lines.begin();
			i != m_Lines.end(); ++i)
			{
		i->Position.y -= 20;
		i->Color.Set(i->Color.GetRed(), i->Color.GetGreen(), i->Color.GetBlue(),
				i->Color.GetAlpha() - 20);
	}

	// Et on cr�e la nouvelle
	//TODO : Add Color
	m_Lines.push_front(CGraphicString(Math::TVector2F(10, 190), Line, s_Fonts[m_CurrentFont], 20));
}

////////////////////////////////////////////////////////////
/// S�lectionne la police suivante
///
/// \return Nom de la nouvelle police
///
////////////////////////////////////////////////////////////
std::string DefaultLook::NextFont()
{
	m_CurrentFont = (m_CurrentFont + 1) % (sizeof(s_Fonts) / sizeof(*s_Fonts));

	return "Nouvelle police : \"" + s_Fonts[m_CurrentFont] + "\"";
}
}
