/*
 * AdaptationLuminanceSimple.h
 *
 *  Created on: Jun 10, 2011
 *      Author: adrien
 */

#ifndef ADAPTATIONLUMINANCESIMPLE_H_
#define ADAPTATIONLUMINANCESIMPLE_H_

#include <Addons/ToneMappingOperator/ManualMipmapping.h>

class AdaptationLuminanceSimple {
protected:
	float m_LuminanceValue;
public:
	AdaptationLuminanceSimple();
	virtual ~AdaptationLuminanceSimple();

	virtual void UpdateLuminance(ManualMipmapping* mipmapping);
	float GetAdaptationLuminance() const;
};

#endif /* ADAPTATIONLUMINANCESIMPLE_H_ */
