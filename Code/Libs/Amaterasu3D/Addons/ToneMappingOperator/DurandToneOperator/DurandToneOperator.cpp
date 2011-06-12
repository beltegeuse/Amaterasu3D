/*
 * DurandToneOperator.cpp
 *
 *  Created on: Jun 12, 2011
 *      Author: adrien
 */

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
