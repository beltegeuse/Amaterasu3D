//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================

#ifndef BLEACHINGFILTER_H_
#define BLEACHINGFILTER_H_

namespace ama3D
{
class BleachingFilter
{
protected:
	float m_LumDes;
	float m_TimeRecover;
	float m_Output;
	float m_NewValue;
public:
	BleachingFilter(float lumDestroy, float timeRecover, float initialValue =
			0.9) :
			m_LumDes(lumDestroy), m_TimeRecover(timeRecover), m_NewValue(
					initialValue)
	{
	}

	virtual ~BleachingFilter()
	{
	}

	void Update(float delta, float Ain)
	{
		m_Output = m_NewValue;
		float J = delta * (m_Output / m_LumDes) * Ain;
		float K = delta * (1 - m_Output) / m_TimeRecover;
		m_NewValue = std::max(std::min((K - J) + m_Output, 1.f), 0.f);
	}

	float GetValue() const
	{
		return m_Output;
	}
};
}

#endif /* BLEACHINGFILTER_H_ */
