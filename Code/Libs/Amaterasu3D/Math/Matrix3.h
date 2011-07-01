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
#pragma once

namespace ama3D
{
namespace Math
{
class CMatrix3
{
public:
	CMatrix3(float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m21 =
			0.0f, float m22 = 1.0f, float m23 = 0.0f, float m31 = 0.0f,
			float m32 = 0.0f, float m33 = 1.0f);

	operator float*();
	operator const float*() const;

	float a11, a12, a13; ///< 1ere ligne de la matrice
	float a21, a22, a23; ///< 2eme ligne de la matrice
	float a31, a32, a33; ///< 3eme ligne de la matrice
};

std::istream& operator >>(std::istream& Stream, CMatrix3& Mat);
std::ostream& operator <<(std::ostream& Stream, const CMatrix3& Mat);

#include "Matrix3.inl"
}
}
