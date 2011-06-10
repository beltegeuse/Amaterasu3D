/*
 * AdaptationLuminanceSimpleExp.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#include "AdaptationLuminanceSimpleExp.h"

AdaptationLuminanceSimpleExp::AdaptationLuminanceSimpleExp(float alpha) :
m_OldLumianceValue(1.0),
m_Alpha(alpha)
{
}

AdaptationLuminanceSimpleExp::~AdaptationLuminanceSimpleExp()
{
}

void AdaptationLuminanceSimpleExp::UpdateLuminance(ManualMipmapping* mipmapping)
{
	m_OldLumianceValue = m_LuminanceValue;
	AdaptationLuminanceSimple::UpdateLuminance(mipmapping);
	m_LuminanceValue = (m_Alpha)*m_OldLumianceValue + (1.0 - m_Alpha)*m_LuminanceValue;
}
