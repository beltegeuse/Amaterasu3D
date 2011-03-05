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
#include "GraphicString.h"

#include <Graphics/Font/FontManager.h>


//namespace Yes
//{

/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
/// \param StringPosition : Position
/// \param StringText :     Texte
/// \param StringColor :    Couleur
/// \param StringFont :     Police
/// \param StringSize :     Taille
///
////////////////////////////////////////////////////////////
CGraphicString::CGraphicString(const Math::TVector2F& StringPosition, const std::string& StringText,  const std::string& StringFont) :
Position(StringPosition),
Text(StringText)
{
	texture = CFontManager::Instance().GetTexture(StringFont);
}

CGraphicString::~CGraphicString()
{
	CFontManager::Instance().DeleteTexture(texture);
}


/////////////////////////////////////////////////////////////
/// Affiche la cha�ne de caract�res � l'�cran
///
////////////////////////////////////////////////////////////
void CGraphicString::Draw()
{
    CFontManager::Instance().rat_texture_font_render_text(texture, Position.x, Position.y, const_cast<char*>(Text.c_str()));
}

void CGraphicString::SetString(std::string text){
	Text = text;
}

