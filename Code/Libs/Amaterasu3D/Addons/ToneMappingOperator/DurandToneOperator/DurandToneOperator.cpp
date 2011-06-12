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
	/*uniform vec2 SizeTexture;
	uniform int HalfSizeNeigbours;
	uniform float SigS;
	uniform float SigR;*/

	m_BilateralShader->End();

	m_DurandShader->Begin();
	m_DurandShader->End();
}

void DurandToneOperator::DrawDebug()
{
	m_DurandShader->GetFBO()->DrawDebug();
}
