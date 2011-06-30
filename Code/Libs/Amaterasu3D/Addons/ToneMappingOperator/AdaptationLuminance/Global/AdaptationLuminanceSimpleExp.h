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

#ifndef ADAPTATIONLUMINANCESIMPLEEXP_H_
#define ADAPTATIONLUMINANCESIMPLEEXP_H_

#include <Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.h>
#include <System/EventListeners.h>
class AdaptationLuminanceSimpleExp : public AdaptationLuminanceSimple, public FrameListener
{
protected:
	float m_OldLumianceValue;
	float m_T0;
	float m_Delta;
public:
	AdaptationLuminanceSimpleExp(float To = 2.0);
	virtual ~AdaptationLuminanceSimpleExp();

	virtual void UpdateLuminance(ManualMipmapping* mipmapping);
	virtual void FrameStarted(double delta);
	virtual void FrameEnded() {}
};

#endif /* ADAPTATIONLUMINANCESIMPLEEXP_H_ */
