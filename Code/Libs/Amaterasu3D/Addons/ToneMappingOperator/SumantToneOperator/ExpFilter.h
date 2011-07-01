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

#ifndef EXPFILTER_H_
#define EXPFILTER_H_

namespace ama3D
{
class ExpFilter
{
private:
	float m_Output;
	float m_T0;
public:
	ExpFilter(float T0, float initialV = 1.0) :
			m_T0(T0), m_Output(initialV)
	{
	}

	virtual ~ExpFilter()
	{
	}

	void Update(float in, float delta)
	{
		float alpha = delta / (m_T0 + delta);
		m_Output = (1.0 - alpha) * m_Output + alpha * in;
	}

	float GetValue() const
	{
		return m_Output;
	}
};
}

#endif /* EXPFILTER_H_ */
