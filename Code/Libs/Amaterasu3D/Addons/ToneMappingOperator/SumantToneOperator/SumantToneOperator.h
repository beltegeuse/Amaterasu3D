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

#ifndef SUMANTTONEOPERATOR_H_
#define SUMANTTONEOPERATOR_H_

#include <System/EventListeners.h>
#include <Addons/ToneMappingOperator/AbsrtactToneOperator.h>
#include <Addons/ToneMappingOperator/SumantToneOperator/BleachingFilter.h>
#include <Addons/ToneMappingOperator/SumantToneOperator/ExpFilter.h>
#include <Graphics/Shaders/Tools/ManualMipmapping.h>

namespace ama3D
{
class SumantToneOperator: public FrameListener, public AbsrtactToneOperator
{
protected:
	/*
	 * Attributes
	 */

	// Time
	float m_Delta;

	// Mipmapping to compute Average
	ManualMipmapping m_ManualMipmapping;

	// Filters
	BleachingFilter m_RodBlenching;
	BleachingFilter m_ConeBlenching;
	ExpFilter m_ArodFilter;
	ExpFilter m_AconeFilter;

	// Shader for Tone Operator
	TShaderPtr m_ToneOperator;

public:
	SumantToneOperator();
	virtual ~SumantToneOperator();

	// From AbstractTOneOperator
	virtual Texture* GetLDRTexture();
	virtual void Compress(Texture* HDRBuffer);
	virtual void DrawDebug();

	// From FrameListener
	virtual void FrameStarted(double delta)
	{
		m_Delta = delta;
	}
	virtual void FrameEnded()
	{
	}
};
}

#endif /* SUMANTTONEOPERATOR_H_ */
