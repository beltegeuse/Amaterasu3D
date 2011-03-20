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
// En-têtes
//==========================================================
#include <Graphics/Font/GraphicString.h>
#include <Addons/Console/Look.h>
#include <Math/Matrix4.h>
#include <list>
#include <Graphics/Texture.h>


////////////////////////////////////////////////////////////
/// Classe définissant un look personnalisé pour la console
////////////////////////////////////////////////////////////
class DefaultLook : public Console::ILook
{
public :

    //----------------------------------------------------------
    // Constructeur par défaut
    //----------------------------------------------------------
    DefaultLook();

    //----------------------------------------------------------
    // Fonction appelée lors de la mise à jour de la console
    //----------------------------------------------------------
    virtual void Update();

    //----------------------------------------------------------
    // Fonction appelée lors de l'affichage de la console
    //----------------------------------------------------------
    virtual void Draw(); // FIXME add CONST

    //----------------------------------------------------------
    // Fonction appelée lors de l'activation / désactivation de la console
    //----------------------------------------------------------
    virtual void Show(bool Visible);

    //----------------------------------------------------------
    // Fonction appelée après l'appel à une commande
    //----------------------------------------------------------
    virtual void CommandCalled(const std::string& Result);

    //----------------------------------------------------------
    // Fonction appelée à chaque changement de la ligne courante
    //----------------------------------------------------------
    virtual void TextChanged(const std::string& NewText);

    //----------------------------------------------------------
    // Fonction appelée en cas d'erreur
    //----------------------------------------------------------
    virtual void Error(const std::string& Message);

private :

    //----------------------------------------------------------
    // Types
    //----------------------------------------------------------
    enum TState {STOPPED, HIDDING, SHOWING};

    //----------------------------------------------------------
    // Ajoute une ligne
    //----------------------------------------------------------
    void AddLine(const std::string& Line = "> ", const CColor& Color = CColor::White);

    //----------------------------------------------------------
    // Sélectionne la police suivante
    //----------------------------------------------------------
    std::string  NextFont();

    //----------------------------------------------------------
    // Données membres
    //----------------------------------------------------------
    static const std::string       s_Fonts[];     ///< Liste des polices disponibles
    std::list<CGraphicString> m_Lines;       ///< Dernières lignes affichées sur la console
    Math::CMatrix4                  m_Transfo;     ///< Matrice de transformation associée à l'affichage de la console
    TState                         m_State;       ///< "Etat" de l'animation de la console
    std::size_t                    m_CurrentFont; ///< Police actuellement utilisée
    TTexturePtr					   m_BackgroundTexture;
};

#endif // MYCONSOLE_H
