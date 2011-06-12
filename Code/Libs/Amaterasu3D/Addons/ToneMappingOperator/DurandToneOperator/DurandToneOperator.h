/*
 * DurandToneOperator.h
 *
 *  Created on: Jun 12, 2011
 *      Author: adrien
 */

#ifndef DURANDTONEOPERATOR_H_
#define DURANDTONEOPERATOR_H_

#include <Addons/ToneMappingOperator/AbsrtactToneOperator.h>

class DurandToneOperator : public AbsrtactToneOperator
{
protected:
	/*
	 * Attributes
	 */
	TShaderPtr m_BilateralShader;
	TShaderPtr m_DurandShader;
public:
	DurandToneOperator();
	virtual ~DurandToneOperator();

	virtual Texture* GetLDRTexture();
	virtual void Compress(Texture* HDRBuffer);
	virtual void DrawDebug();
};

#endif /* DURANDTONEOPERATOR_H_ */
