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

#include "AdaptationLuminanceSimple.h"

AdaptationLuminanceSimple::AdaptationLuminanceSimple() :
		m_LuminanceValue(1.0)
{
}

AdaptationLuminanceSimple::~AdaptationLuminanceSimple()
{
}

void AdaptationLuminanceSimple::UpdateLuminance(ManualMipmapping* mipmapping)
{
	mipmapping->GetLevel(mipmapping->NumberLevels() - 1)->activateTextureMapping();
	mipmapping->GetLevel(mipmapping->NumberLevels() - 1)->activateTexture();
	float values[3];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &values);
	Logger::Log() << "Value : " << values[0] << "x" << values[1] << "x"
			<< values[2] << "\n";
	m_LuminanceValue = values[0] * 0.2126 + values[1] * 0.7152
			+ values[2] * 0.0722;
	Logger::Log() << "Luminance : " << m_LuminanceValue << "\n";
	mipmapping->GetLevel(mipmapping->NumberLevels() - 1)->desactivateTextureMapping();
}

float AdaptationLuminanceSimple::GetAdaptationLuminance() const
{
	return m_LuminanceValue;
}
