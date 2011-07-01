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

#include "PerformancePanel.h"

// STL includes
#include <sstream>

// Amaterasu includes
#include <Debug/Exceptions.h>

namespace ama3D
{
PerformancePanel::PerformancePanel() :
		m_Text(0), m_InStepCounting(false)
{
}

PerformancePanel::~PerformancePanel()
{
}

/*
 * Performances methods
 */
void PerformancePanel::BeginPerformanceCounter()
{
	m_Steps.erase(m_Steps.begin(), m_Steps.end());
	m_TimeStartPerformance = std::clock();
}

void PerformancePanel::EndPerformanceCounter()
{
	Assert(!m_InStepCounting);
	m_Totaltime = (std::clock() - m_TimeStartPerformance);
}

void PerformancePanel::BeginStep(const std::string& stepName)
{
	// Protection
	Assert(!m_InStepCounting);
	m_InStepCounting = true;

	// Start counting
	m_TimeStartSteps = std::clock();

	// Add step
	PerformancePanel::Steps s;
	s.Name = stepName;
	s.Time = 0.0;
	m_Steps.push_back(s);
}

void PerformancePanel::EndStep()
{
	// Protection
	Assert(m_InStepCounting);
	m_InStepCounting = false;

	// Compute the time
	m_Steps.back().Time = (std::clock() - m_TimeStartSteps);
}

void PerformancePanel::Draw()
{
	if (m_Text == 0)
		m_Text = new CGraphicString;

	m_Text->Text = "Steps : ";
	m_Text->Position = Math::TVector2F(0, 0);
	m_Text->Draw();

	float cumulatedTime = 0.0;
	for (unsigned int i = 0; i < m_Steps.size(); i++)
	{
		std::stringstream ss;
		cumulatedTime += m_Steps[i].Time;
		ss << " * " << m_Steps[i].Name << " : " << m_Steps[i].Time << " ms";
		m_Text->Text = ss.str();
		m_Text->Position = Math::TVector2F(10, 18 * (i + 1));
		m_Text->Draw();
	}

	{
		std::stringstream ss;
		ss << "Other : " << m_Totaltime - cumulatedTime << " ms";
		m_Text->Text = ss.str();
		m_Text->Position = Math::TVector2F(0, 18 * (m_Steps.size() + 1));
		m_Text->Draw();
	}
	{
		std::stringstream ss;
		ss << "Total time : " << m_Totaltime << " ms";
		m_Text->Text = ss.str();
		m_Text->Position = Math::TVector2F(0, 18 * (m_Steps.size() + 2));
		m_Text->Draw();
	}
}
}
