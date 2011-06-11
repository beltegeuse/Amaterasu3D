/*
 * ExpFilter.h
 *
 *  Created on: Jun 11, 2011
 *      Author: adrien
 */

#ifndef EXPFILTER_H_
#define EXPFILTER_H_

class ExpFilter {
private:
	float m_Output;
	float m_T0;
public:
	ExpFilter(float T0, float initialV = 1.0) :
		m_T0(T0),
		m_Output(initialV)
	{
	}

	virtual ~ExpFilter() {}

	void Update(float in, float delta)
	{
		float alpha = delta/(m_T0+delta);
		m_Output = (1.0-alpha)*m_Output + alpha*in;
	}

	float GetValue() const { return m_Output; }
};

#endif /* EXPFILTER_H_ */
