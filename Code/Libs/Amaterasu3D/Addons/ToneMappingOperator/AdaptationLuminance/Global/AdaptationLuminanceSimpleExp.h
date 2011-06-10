/*
 * AdaptationLuminanceSimpleExp.h
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#ifndef ADAPTATIONLUMINANCESIMPLEEXP_H_
#define ADAPTATIONLUMINANCESIMPLEEXP_H_

#include <Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.h>

class AdaptationLuminanceSimpleExp : public AdaptationLuminanceSimple
{
protected:
	float m_OldLumianceValue;
public:
	AdaptationLuminanceSimpleExp();
	virtual ~AdaptationLuminanceSimpleExp();

	virtual void UpdateLuminance(ManualMipmapping* mipmapping);
};

#endif /* ADAPTATIONLUMINANCESIMPLEEXP_H_ */
