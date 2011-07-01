//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
// E-mail : adrien.gruson@gmail.com
//==========================================================

#include "Logo.h"

#include <System/SettingsManager.h>

namespace ama3D
{
Logo::Logo() :
		m_Rectangle(0)
{
}

Logo::~Logo()
{
}

void Logo::Draw()
{
	// On initialise au premier appel
	if (m_Rectangle == 0)
	{
		Math::TVector2I screenSize = Math::TVector2I(
				CSettingsManager::Instance().GetSizeRenderingWindow());
		m_Rectangle = new Rectangle2D(screenSize - Math::TVector2I(240, 160),
				screenSize - Math::TVector2I(10, 10), true);
		m_Rectangle->GetObject().AddTextureMap(DIFFUSE_TEXTURE,
				Texture::LoadFromFile("logo.png"));
		m_2DShader = CShaderManager::Instance().LoadShader("2DDraw.shader");
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_2DShader->Begin();
	m_Rectangle->Render();
	m_2DShader->End();
	glDisable(GL_BLEND);
}
}
