/*
 * AdaptationLuminanceSimpleExp.h
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#ifndef ADAPTATIONLUMINANCESIMPLEEXP_H_
#define ADAPTATIONLUMINANCESIMPLEEXP_H_

#include <Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.h>
#include <System/EventListeners.h>
class AdaptationLuminanceSimpleExp : public AdaptationLuminanceSimple, public FrameListener
{
protected:
	float m_OldLumianceValue;
	float m_T0;
	float m_Delta;
public:
	AdaptationLuminanceSimpleExp(float To = 0.15);
	virtual ~AdaptationLuminanceSimpleExp();

	virtual void UpdateLuminance(ManualMipmapping* mipmapping);
	virtual void FrameStarted(double delta);
	virtual void FrameEnded() {}
};

#endif /* ADAPTATIONLUMINANCESIMPLEEXP_H_ */
