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

#ifndef MATRIX2_H_
#define MATRIX2_H_

#include <Math/Vector2.h>

namespace ama3D
{
namespace Math
{

class Matrix2
{
public:
	Matrix2(float a11 = 1.f, float a12 = 0.f,
			float a21 = 0.f, float a22 = 1.f);
	virtual ~Matrix2();

	TVector2F Transform(const TVector2F& v) const;

	// Access to the data
	float& operator ()(std::size_t i, std::size_t j);
	const float& operator ()(std::size_t i, std::size_t j) const;

	// Access to the data (array)
	operator float*();
	operator const float*() const;

	// Attributes
	float a11, a12; ///< 1ere ligne de la matrice
	float a21, a22; ///< 2eme ligne de la matrice
};

#include <Math/Matrix2.inl>

} /* namespace Math */
} /* namespace ama3D */
#endif /* MATRIX2_H_ */
