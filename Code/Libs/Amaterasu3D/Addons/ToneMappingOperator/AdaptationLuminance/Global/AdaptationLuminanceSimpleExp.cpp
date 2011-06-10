/*
 * AdaptationLuminanceSimpleExp.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#include "AdaptationLuminanceSimpleExp.h"

AdaptationLuminanceSimpleExp::AdaptationLuminanceSimpleExp() :
m_OldLumianceValue(1.0)
{
}

AdaptationLuminanceSimpleExp::~AdaptationLuminanceSimpleExp()
{
}

void AdaptationLuminanceSimpleExp::UpdateLuminance(ManualMipmapping* mipmapping)
{
	m_OldLumianceValue = m_LuminanceValue;
	AdaptationLuminanceSimple::UpdateLuminance(mipmapping);
	m_LuminanceValue = (0.9)*m_OldLumianceValue + (0.1)*m_LuminanceValue;
}
