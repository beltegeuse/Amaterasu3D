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

#include "PhotographicToneOperator.h"

PhotographicToneOperator::PhotographicToneOperator() :
	AbsrtactToneOperator(),
	m_ManualMipmapping(128),
	m_AdaptationLum(0)
{
	m_PhotoShader = CShaderManager::Instance().LoadShader("PhotoToneOperator.shader");
	m_ManualMipmapping.Initialize();
	m_AdaptationLum = new AdaptationLuminanceSimple;
}

PhotographicToneOperator::~PhotographicToneOperator()
{
	delete m_AdaptationLum;
}

Texture* PhotographicToneOperator::GetLDRTexture()
{
	return m_PhotoShader->GetFBO()->GetTexture("Result");
}

void PhotographicToneOperator::Compress(Texture* HDRBuffer)
{
	m_ManualMipmapping.Compute(HDRBuffer);

	m_AdaptationLum->UpdateLuminance(&m_ManualMipmapping);

	m_PhotoShader->Begin();
	HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	m_PhotoShader->SetUniform1f("AdaptationLum", m_AdaptationLum->GetAdaptationLuminance());
	glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_PhotoShader->End();
}

void PhotographicToneOperator::DrawDebug()
{
	m_PhotoShader->GetFBO()->DrawDebug();
}
