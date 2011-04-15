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

#ifndef PERFORMANCEPANEL_H_
#define PERFORMANCEPANEL_H_

// STL includes
#include <vector>
#include <string>
#include <ctime>

// Amaterasu includes
#include <Graphics/Font/GraphicString.h>

class PerformancePanel
{
private:
	/*
	 * Other private structures
	 */
	struct Steps
	{
		std::string Name;
		float Time;
	};
	/*
	 * Attributes
	 */
	std::vector<Steps> m_Steps;
	CGraphicString* m_Text;
	bool m_InStepCounting;
	std::clock_t m_TimeStartPerformance;
	std::clock_t m_TimeStartSteps;
	float m_Totaltime; ///< In millisecondes
public:
	/*
	 * Constructors and destructors
	 */
	PerformancePanel();
	virtual ~PerformancePanel();

	/*
	 * Public methods
	 */
	// Performances methods
	void BeginPerformanceCounter();
	void EndPerformanceCounter();
	void BeginStep(const std::string& stepName);
	void EndStep();
	// Others methods
	void Draw(); ///< To draw the panel
};

#endif /* PERFORMANCEPANEL_H_ */