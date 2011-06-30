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

#ifndef MYCONSOLE_H
#define MYCONSOLE_H

//==========================================================
// En-t�tes
//==========================================================
#include <Graphics/Font/GraphicString.h>
#include <Graphics/SceneNode/SimpleRenderable/Rectangle2D.h>
#include <Graphics/Shaders/Shader.h>
#include <Addons/Console/Look.h>
#include <Math/Matrix4.h>
#include <list>
#include <Graphics/Texture.h>

////////////////////////////////////////////////////////////
/// Classe d�finissant un look personnalis� pour la console
////////////////////////////////////////////////////////////
class DefaultLook: public Console::ILook
{
public:

	//----------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------
	DefaultLook();
	virtual ~DefaultLook();
	//----------------------------------------------------------
	// Fonction appel�e lors de la mise � jour de la console
	//----------------------------------------------------------
	void Update(double delta);

	//----------------------------------------------------------
	// Fonction appel�e lors de l'affichage de la console
	//----------------------------------------------------------
	virtual void Draw(); // FIXME add CONST

	//----------------------------------------------------------
	// Fonction appel�e lors de l'activation / d�sactivation de la console
	//----------------------------------------------------------
	virtual void Show(bool Visible);

	//----------------------------------------------------------
	// Fonction appel�e apr�s l'appel � une commande
	//----------------------------------------------------------
	virtual void CommandCalled(const std::string& Result);

	//----------------------------------------------------------
	// Fonction appel�e � chaque changement de la ligne courante
	//----------------------------------------------------------
	virtual void TextChanged(const std::string& NewText);

	//----------------------------------------------------------
	// Fonction appel�e en cas d'erreur
	//----------------------------------------------------------
	virtual void Error(const std::string& Message);

private:

	//----------------------------------------------------------
	// Types
	//----------------------------------------------------------
	enum TState
	{
		STOPPED, HIDDING, SHOWING
	};

	//----------------------------------------------------------
	// Ajoute une ligne
	//----------------------------------------------------------
	void AddLine(const std::string& Line = "> ", const CColor& Color =
			CColor::White);

	//----------------------------------------------------------
	// S�lectionne la police suivante
	//----------------------------------------------------------
	std::string NextFont();

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	static const std::string s_Fonts[]; ///< Liste des polices disponibles
	std::list<CGraphicString> m_Lines; ///< Derni�res lignes affich�es sur la console
	Math::CMatrix4 m_Transfo; ///< Matrice de transformation associ�e � l'affichage de la console
	TState m_State; ///< "Etat" de l'animation de la console
	std::size_t m_CurrentFont; ///< Police actuellement utilis�e
	TTexturePtr m_BackgroundTexture;
	TShaderPtr m_2DShader;
	Rectangle2D* m_Rectangle;
	bool m_ShowText;
	float m_Height;
};

#endif // MYCONSOLE_H
