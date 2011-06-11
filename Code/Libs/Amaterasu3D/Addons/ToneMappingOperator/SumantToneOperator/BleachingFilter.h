/*
 * BleachingFilter.h
 *
 *  Created on: Jun 11, 2011
 *      Author: adrien
 */

#ifndef BLEACHINGFILTER_H_
#define BLEACHINGFILTER_H_

class BleachingFilter {
protected:
	float m_LumDes;
	float m_TimeRecover;
	float m_Output;
	float m_NewValue;
public:
	BleachingFilter(float lumDestroy, float timeRecover, float initialValue = 0.9) :
		m_LumDes(lumDestroy),
		m_TimeRecover(timeRecover),
		m_NewValue(initialValue)
	{
	}

	virtual ~BleachingFilter() {}

	void Update(float delta, float Ain)
	{
		m_Output = m_NewValue;
		float J = delta*(m_Output/m_LumDes)*Ain;
		float K = delta*(1 - m_Output)/m_TimeRecover;
		m_NewValue = std::max(std::min((K-J) + m_Output,1.f),0.f);
	}

	float GetValue() const
	{
		return m_Output;
	}
};

#endif /* BLEACHINGFILTER_H_ */
