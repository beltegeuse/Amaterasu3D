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

PhotographicToneOperator::PhotographicToneOperator(Texture* HDRBuffer) :
	AbsrtactToneOperator(HDRBuffer)
{
	m_LuminanceShader = CShaderManager::Instance().LoadShader("ComputeLuminance.shader");
	m_PhotoShader = CShaderManager::Instance().LoadShader("PhotoToneOperator.shader");
}

PhotographicToneOperator::~PhotographicToneOperator()
{
}

Texture* PhotographicToneOperator::GetLDRTexture()
{
	return m_PhotoShader->GetFBO()->GetTexture("Result");
}

void PhotographicToneOperator::Compress()
{
	m_LuminanceShader->Begin();
	m_HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	m_HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_LuminanceShader->End();

	m_PhotoShader->Begin();
	m_HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	m_LuminanceShader->GetFBO()->GetTexture("Result")->activateMultiTex(CUSTOM_TEXTURE+1);
	glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	m_HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_LuminanceShader->GetFBO()->GetTexture("Result")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_PhotoShader->End();
}

void PhotographicToneOperator::DrawDebug()
{
	m_PhotoShader->GetFBO()->DrawDebug();
}
