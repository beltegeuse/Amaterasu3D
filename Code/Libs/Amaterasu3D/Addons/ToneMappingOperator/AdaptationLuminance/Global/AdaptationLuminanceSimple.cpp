/*
 * AdaptationLuminanceSimple.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#include "AdaptationLuminanceSimple.h"

AdaptationLuminanceSimple::AdaptationLuminanceSimple() :
m_LuminanceValue(1.0)
{
}

AdaptationLuminanceSimple::~AdaptationLuminanceSimple()
{
}


void AdaptationLuminanceSimple::UpdateLuminance(ManualMipmapping* mipmapping)
{
	mipmapping->GetLevel(mipmapping->NumberLevels()-1)->activateTextureMapping();
	mipmapping->GetLevel(mipmapping->NumberLevels()-1)->activateTexture();
	float values[3];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB,GL_FLOAT,&values);
	Logger::Log() << "Value : " << values[0] << "x" << values[1] << "x" << values[2] << "\n";
	m_LuminanceValue = values[0]*0.2126 + values[1]*0.7152 + values[2]*0.0722 ;
	Logger::Log() << "Luminance : " << m_LuminanceValue << "\n";
	mipmapping->GetLevel(mipmapping->NumberLevels()-1)->desactivateTextureMapping();
}

float AdaptationLuminanceSimple::GetAdaptationLuminance() const
{
	return m_LuminanceValue;
}
