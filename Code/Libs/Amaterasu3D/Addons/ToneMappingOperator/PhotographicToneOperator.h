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

#ifndef PHOTOGRAPHICTONEOPERATOR_H_
#define PHOTOGRAPHICTONEOPERATOR_H_

#include <Addons/ToneMappingOperator/AbsrtactToneOperator.h>
#include <Graphics/Shaders/Shader.h>

class PhotographicToneOperator : public AbsrtactToneOperator
{
protected:
	TShaderPtr m_LuminanceShader;
	TShaderPtr m_PhotoShader;
public:
	PhotographicToneOperator(Texture* HDRBuffer);
	virtual ~PhotographicToneOperator();

	virtual Texture* GetLDRTexture();
	virtual void Compress();
	virtual void DrawDebug();
};

#endif /* PHOTOGRAPHICTONEOPERATOR_H_ */
