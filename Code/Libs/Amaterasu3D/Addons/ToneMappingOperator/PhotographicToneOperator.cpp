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
	AbsrtactToneOperator(HDRBuffer),
	m_ManualMipmapping(128)
{
	m_PhotoAdaptationBuffer = CShaderManager::Instance().LoadShader("PhotoAdaptationLumSimple.shader");
	m_PhotoShader = CShaderManager::Instance().LoadShader("PhotoToneOperator.shader");
	m_ManualMipmapping.Initialize();
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
	m_ManualMipmapping.Compute(m_HDRBuffer);

	// Try something
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->activateTextureMapping();
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->activateTexture();
	float lum = 0;
	glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE,GL_FLOAT,&lum);
	Logger::Log() << "Luminance : " << lum << "\n";
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->desactivateTextureMapping();

	m_PhotoAdaptationBuffer->Begin();
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->activateMultiTex(CUSTOM_TEXTURE);
	glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->desactivateMultiTex(CUSTOM_TEXTURE);
	m_PhotoAdaptationBuffer->End();

	m_PhotoShader->Begin();
	m_HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	m_PhotoAdaptationBuffer->GetFBO()->GetTexture("Result")->activateMultiTex(CUSTOM_TEXTURE+1);
	//m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->activateMultiTex(CUSTOM_TEXTURE+1);
	glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	m_HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	//m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_PhotoAdaptationBuffer->GetFBO()->GetTexture("Result")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_PhotoShader->End();
}

void PhotographicToneOperator::DrawDebug()
{
	m_PhotoShader->GetFBO()->DrawDebug();
}
