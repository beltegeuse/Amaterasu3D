/*
 * PhotographicToneOperatorExp.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#include "PhotographicToneOperatorExp.h"
#include <Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.h>
PhotographicToneOperatorExp::PhotographicToneOperatorExp()
{
	delete m_AdaptationLum;
	m_AdaptationLum = new AdaptationLuminanceSimpleExp();
}

PhotographicToneOperatorExp::~PhotographicToneOperatorExp() {
}
