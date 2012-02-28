/*
 * CatmullRomInterpolator.h
 *
 *  Created on: Jun 7, 2011
 *      Author: adrien
 */

#ifndef CATMULLROMINTERPOLATOR_H_
#define CATMULLROMINTERPOLATOR_H_

#include <vector>

namespace ama3D
{
template<class T> class CatmullRomInterpolator
{
protected:
	/*
	 * Attributes
	 */
	std::vector<T> m_Points;

	/*
	 * Private methods
	 */
	T ComputeInterpolation(T& p0, T& p1, T& p2, T& p3, float t)
	{
		return ((2.f * p1) + (p2 + (-p0)) * t
				+ (p0 * 2.f + p1 * (-5.f) + p2 * 4.f + p3 * (-1.f)) * t * t
				+ (p0 * (-1.f) + p1 * 3.f + (-3.f) * p2 + p3) * t * t * t)
				* 0.5f;
	}
public:
	/*
	 * Constructor & Destructors
	 */
	CatmullRomInterpolator()
	{
	}
	virtual ~CatmullRomInterpolator()
	{
	}

	/*
	 * Points managements
	 */
	void AddPoint(const T& v)
	{
		if (m_Points.empty())
		{
			m_Points.push_back(v);
			m_Points.push_back(v);
		}
		m_Points[m_Points.size() - 1] = v;
		m_Points.push_back(v);
	}
	void EraseAllPoints()
	{
		m_Points.erase(m_Points.begin(), m_Points.end());
	}

	T Interpolation(float t)
	{
		Assert(
				t >= 0.0 && (t + 1) <= (m_Points.size() + 2)
						&& m_Points.size() > 4);

		int id = floor(t);

		return ComputeInterpolation(m_Points[id], m_Points[id + 1],
				m_Points[id + 2], m_Points[id + 3], t - id);
	}
};
}

#endif /* CATMULLROMINTERPOLATOR_H_ */
