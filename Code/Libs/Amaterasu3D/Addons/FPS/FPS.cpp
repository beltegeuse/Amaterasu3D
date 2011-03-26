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

#include "FPS.h"

#include <Logger/Logger.h>

FPS::FPS() :
	m_TimeElasped(0.0),
	m_FPS(0)
{
}

FPS::~FPS()
{
}

void FPS::FrameStarted(double delta)
{
	// Time update
	m_TimeElasped += delta;
}

void FPS::FrameEnded()
{
	m_FPS++;
	if(m_TimeElasped > 1.0)
	{
		Logger::Log() << "[INFO] FPS : " << m_FPS << "\n";
		// Reinitialize all variables
		m_FPS = 0;
		m_TimeElasped -= 1.0;
	}
}