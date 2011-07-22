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

inline Matrix2::Matrix2(float a11, float a12,
				 float a21, float a22) :
		a11(a11), a12(a12), a21(a21), a22(a22)
{
}

inline Matrix2::~Matrix2()
{
}

TVector2F Matrix2::Transform(const TVector2F& v) const
{
	return TVector2F(a11*v.x + a12*v.y, a21*v.x + a22*v.y);
}

inline float& Matrix2::operator ()(std::size_t i, std::size_t j)
{
	return (*this)[i+2*j];
}

inline const float& Matrix2::operator ()(std::size_t i, std::size_t j) const
{
	return (*this)[i+2*j];
}

inline Matrix2::operator float*()
{
	return &a11;
}

inline Matrix2::operator const float*() const
{
	return &a11;
}

