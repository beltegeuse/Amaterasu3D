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

#include "AdaptationLuminanceSimpleExp.h"

AdaptationLuminanceSimpleExp::AdaptationLuminanceSimpleExp(float To) :
		m_OldLumianceValue(1.0), m_T0(To)
{
}

AdaptationLuminanceSimpleExp::~AdaptationLuminanceSimpleExp()
{
}

void AdaptationLuminanceSimpleExp::FrameStarted(double delta)
{
	m_Delta = delta;
}

void AdaptationLuminanceSimpleExp::UpdateLuminance(ManualMipmapping* mipmapping)
{
	m_OldLumianceValue = m_LuminanceValue;
	AdaptationLuminanceSimple::UpdateLuminance(mipmapping);
	float alpha = m_Delta / (m_T0 + m_Delta);
	m_LuminanceValue = (1.0 - alpha) * m_OldLumianceValue
			+ alpha * m_LuminanceValue;
}
