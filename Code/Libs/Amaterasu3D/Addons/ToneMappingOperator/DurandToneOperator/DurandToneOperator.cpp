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

#include "DurandToneOperator.h"

DurandToneOperator::DurandToneOperator()
{
	m_BilateralShader = CShaderManager::Instance().LoadShader("BilateralFiltering.shader");
	m_DurandShader = CShaderManager::Instance().LoadShader("DurandToneMapping.shader");
}

DurandToneOperator::~DurandToneOperator()
{
}

Texture* DurandToneOperator::GetLDRTexture()
{
	return m_DurandShader->GetFBO()->GetTexture("Result");
}

void DurandToneOperator::Compress(Texture* HDRBuffer)
{
	m_BilateralShader->Begin();
	HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	/*uniform vec2 SizeTexture;
	uniform int HalfSizeNeigbours;
	uniform float SigS;
	uniform float SigR;*/
	Math::TVector2I size = CSettingsManager::Instance().GetSizeRenderingWindow();
	int HalfSize = floor(std::min(size.x, size.y)*0.01);

	Logger::Log() << "[DEBUG] Half : " << HalfSize << "\n";

	m_BilateralShader->SetUniformVector("SizeTexture", Math::TVector2F(size.x, size.y));
	m_BilateralShader->SetUniform1i("HalfSizeNeigbours",HalfSize);
	m_BilateralShader->SetUniform1f("SigS", (HalfSize - 1) / 8.0);
	m_BilateralShader->SetUniform1f("SigR", 0.35);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_BilateralShader->End();

	m_DurandShader->Begin();
	HDRBuffer->activateMultiTex(CUSTOM_TEXTURE+0);
	m_BilateralShader->GetFBO()->GetTexture("Result")->activateMultiTex(CUSTOM_TEXTURE+1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_BilateralShader->GetFBO()->GetTexture("Result")->desactivateMultiTex(CUSTOM_TEXTURE+1);
	m_DurandShader->End();
}

void DurandToneOperator::DrawDebug()
{
	m_DurandShader->GetFBO()->DrawDebug();
}
