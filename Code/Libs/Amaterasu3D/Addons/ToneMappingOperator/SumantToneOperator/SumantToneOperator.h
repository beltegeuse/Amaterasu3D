/*
 * SumantToneOperator.h
 *
 *  Created on: Jun 11, 2011
 *      Author: adrien
 */

#ifndef SUMANTTONEOPERATOR_H_
#define SUMANTTONEOPERATOR_H_

#include <System/EventListeners.h>
#include <Addons/ToneMappingOperator/AbsrtactToneOperator.h>
#include <Addons/ToneMappingOperator/SumantToneOperator/BleachingFilter.h>
#include <Addons/ToneMappingOperator/SumantToneOperator/ExpFilter.h>
#include <Addons/ToneMappingOperator/ManualMipmapping.h>

class SumantToneOperator : public FrameListener, public AbsrtactToneOperator
{
protected:
	/*
	 * Attributes
	 */

	// Time
	float m_Delta;

	// Mipmapping to compute Average
	ManualMipmapping m_ManualMipmapping;

	// Filters
	BleachingFilter m_RodBlenching;
	BleachingFilter m_ConeBlenching;
	ExpFilter m_ArodFilter;
	ExpFilter m_AconeFilter;

	// Shader for Tone Operator
	TShaderPtr m_ToneOperator;

public:
	SumantToneOperator();
	virtual ~SumantToneOperator();

	// From AbstractTOneOperator
	virtual Texture* GetLDRTexture();
	virtual void Compress(Texture* HDRBuffer);
	virtual void DrawDebug();

	// From FrameListener
	virtual void FrameStarted(double delta) { m_Delta = delta; }
	virtual void FrameEnded() {}
};

#endif /* SUMANTTONEOPERATOR_H_ */
