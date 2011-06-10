/*
 * AdaptationLuminanceSimpleExp.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#include "AdaptationLuminanceSimpleExp.h"

AdaptationLuminanceSimpleExp::AdaptationLuminanceSimpleExp(float To) :
m_OldLumianceValue(1.0),
m_T0(To)
{
}

AdaptationLuminanceSimpleExp::~AdaptationLuminanceSimpleExp()
{
}

void AdaptationLuminanceSimpleExp::FrameStarted(double delta)
{
	m_Delta = delta;
}

void AdaptationLuminanceSimpleExp::UpdateLuminance(ManualMipmapping* mipmapping)
{
	m_OldLumianceValue = m_LuminanceValue;
	AdaptationLuminanceSimple::UpdateLuminance(mipmapping);
	float alpha = m_Delta/(m_T0+m_Delta);
	m_LuminanceValue = (1.0-alpha)*m_OldLumianceValue + alpha*m_LuminanceValue;
}
