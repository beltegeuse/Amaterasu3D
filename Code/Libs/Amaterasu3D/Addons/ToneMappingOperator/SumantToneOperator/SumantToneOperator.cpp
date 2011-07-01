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

#include "SumantToneOperator.h"

namespace ama3D
{
SumantToneOperator::SumantToneOperator() :
		m_ManualMipmapping(128), m_RodBlenching(16.0, 400), m_ConeBlenching(
				220000000, 110), // 220 000 000 = 2.2*10^8
		m_ArodFilter(0.150), m_AconeFilter(0.08)
{
	m_ToneOperator = CShaderManager::Instance().LoadShader(
			"SumantToneOperator.shader");
	m_ManualMipmapping.Initialize();
}

SumantToneOperator::~SumantToneOperator()
{
}

Texture* SumantToneOperator::GetLDRTexture()
{
	return m_ToneOperator->GetFBO()->GetTexture("Result");
}

void SumantToneOperator::Compress(Texture* HDRBuffer)
{
	m_ManualMipmapping.Compute(HDRBuffer);

	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels() - 1)->activateTextureMapping();
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels() - 1)->activateTexture();
	float averageValues[3];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &averageValues);
	Logger::Log() << "Value : " << averageValues[0] << "x" << averageValues[1]
			<< "x" << averageValues[2] << "\n";
	m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels() - 1)->desactivateTextureMapping();

	// Compute Grod & Gcone (adapatation values)
	float Grod = std::max(
			-0.702 * averageValues[0] + 1.039 * averageValues[1]
					+ 0.433 * averageValues[2], 0.0);
	float Gcone = std::max(
			0.256 * averageValues[0] + 0.67 * averageValues[1]
					+ 0.065 * averageValues[2], 0.0);

	// Update all filters
	m_RodBlenching.Update(m_Delta, Grod);
	m_ConeBlenching.Update(m_Delta, Gcone);
	m_ArodFilter.Update(Grod, m_Delta);
	m_AconeFilter.Update(Gcone, m_Delta);

	// Pour calculs plus faciles a lire
	float Arod = m_ArodFilter.GetValue();
	float Acone = m_AconeFilter.GetValue();

	// Calcul des sigma
	// * For rods
	float j = 1.0 / (500000 * Arod + 1.0);
	float sigRod = (2.5874 * Arod)
			/ (19000 * j * j * Arod
					+ 0.2615 * std::pow(1 - j * j, 4)
							* std::pow(Arod, 1.f / 6.f));
	// * For cones
	float k = 1.0 / (5 * Acone + 1.0);
	float k4 = k * k * k * k;
	float sigCone =
			(12.9223 * Acone)
					/ (k4 * Acone
							+ 0.171 * std::pow(1.0 - k4, 2)
									* std::pow(Acone, 1.f / 3.f));

	Logger::Log() << "[DEBUG] Info operator Sumant : \n";
	Logger::Log() << "    * Grod : " << Grod << "\n";
	Logger::Log() << "    * Gcone : " << Gcone << "\n";
	Logger::Log() << "    * Arod : " << Arod << "\n";
	Logger::Log() << "    * Acone : " << Acone << "\n";
	Logger::Log() << "    * SigRod : " << sigRod << "\n";
	Logger::Log() << "    * SigCone : " << sigCone << "\n";
	Logger::Log() << "    * Brod : " << m_RodBlenching.GetValue() << "\n";
	Logger::Log() << "    * Bcone : " << m_ConeBlenching.GetValue() << "\n";

	m_ToneOperator->Begin();
	HDRBuffer->activateMultiTex(CUSTOM_TEXTURE + 0);
	/*uniform float SigCone;
	 uniform float SigRod;
	 uniform float Arod;
	 uniform float Acone;
	 uniform float Brod;
	 uniform float Bcone;*/
	m_ToneOperator->SetUniform1f("SigCone", sigCone);
	m_ToneOperator->SetUniform1f("SigRod", sigRod);
	m_ToneOperator->SetUniform1f("Arod", Arod);
	m_ToneOperator->SetUniform1f("Acone", Acone);
	m_ToneOperator->SetUniform1f("Brod", m_RodBlenching.GetValue());
	m_ToneOperator->SetUniform1f("Bcone", m_ConeBlenching.GetValue());
	glBegin(GL_QUADS);
	glVertex2f(-1.0, -1.0);
	glVertex2f(-1.0, 1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, -1.0);
	glEnd();
	HDRBuffer->desactivateMultiTex(CUSTOM_TEXTURE + 0);
	m_ToneOperator->End();

}

void SumantToneOperator::DrawDebug()
{
	m_ToneOperator->GetFBO()->DrawDebug();
}
}
