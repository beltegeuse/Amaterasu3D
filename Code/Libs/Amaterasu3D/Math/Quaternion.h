//==========================================================
// Yes::Engine - Free C++ 3D engine
//
// Copyright (C) 2004-2005 Laurent Gomila
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
// E-mail : laurent.gom@gmail.com
//==========================================================

#ifndef QUATERNION_H
#define QUATERNION_H

//==========================================================
// En-t�tes
//==========================================================
#include <Math/Matrix4.h>
#include <Math/SphericalCoordinates.h>

namespace Math
{
////////////////////////////////////////////////////////////
// Classe des quaternions
////////////////////////////////////////////////////////////
class CQuaternion
{
public:

	//----------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------
	CQuaternion(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 1.0f);

	//----------------------------------------------------------
	// Constructeur � partir d'une matrice
	//----------------------------------------------------------
	CQuaternion(const CMatrix4& Matrix);

	//----------------------------------------------------------
	// Constructeur � partir d'un axe et d'un angle
	//----------------------------------------------------------
	CQuaternion(const TVector3F& Axis, float Angle);

	//----------------------------------------------------------
	// Met le quaternion � l'identit�
	//----------------------------------------------------------
	void Identity();

	//----------------------------------------------------------
	// Normalise le quaternion
	//----------------------------------------------------------
	inline void Normalize();

	//----------------------------------------------------------
	// Renvoie le conjugu�
	//----------------------------------------------------------
	CQuaternion Conjugate() const;

	//----------------------------------------------------------
	// Construit une matrice de rotation � partir du quaternion
	//----------------------------------------------------------
	CMatrix4 ToMatrix() const;

	//----------------------------------------------------------
	// Construit le quaternion � partir d'une matrice
	//----------------------------------------------------------
	void FromMatrix(const CMatrix4& Matrix);

	//----------------------------------------------------------
	// Construit le quaternion � partir d'un axe et d'un angle
	//----------------------------------------------------------
	void FromAxisAngle(const TVector3F& Axis, float Angle);

	//----------------------------------------------------------
	// Transforme le quaternion en axe & angle de rotation
	//----------------------------------------------------------
	void ToAxisAngle(TVector3F& Axis, float& Angle) const;

	//----------------------------------------------------------
	// Construit le quaternion a partir de 3 angles d'Euler
	//----------------------------------------------------------
	void FromEulerAngles(float X, float Y, float Z);

	//----------------------------------------------------------
	// Construit le quaternion a d'un vecteur 3D
	//----------------------------------------------------------
	void From3DVector(const TVector3F& v);

	//----------------------------------------------------------
	// Operateur de multiplication
	//----------------------------------------------------------
	CQuaternion operator *(const CQuaternion& Quaternion) const;
	CQuaternion operator *(float scale) const;

	//----------------------------------------------------------
	// Operateur d'addition
	//----------------------------------------------------------
	CQuaternion operator +(const CQuaternion& Quaternion) const;

	//----------------------------------------------------------
	// Operateur de multiplication - affectation
	//----------------------------------------------------------
	const CQuaternion& operator *=(const CQuaternion& Quaternion);

	//----------------------------------------------------------
	// Dot product
	//----------------------------------------------------------
	static inline float Dot(const CQuaternion& q1, const CQuaternion& q2);

	//=========== Interpolation methods
	// Linear interpolation
	static CQuaternion lerp(const CQuaternion &q1, const CQuaternion &q2,
			float t);
	// Linear spherical interpolation
	static CQuaternion slerp(const CQuaternion &q1, const CQuaternion &q2,
			float t);
	// Linear spherical interpolation (no reduce splinning)
	static CQuaternion slerpNoInvert(const CQuaternion &q1,
			const CQuaternion &q2, float t);
	// Cubic spherical interpolation
	static CQuaternion squad(const CQuaternion &q1, const CQuaternion &q2,
			const CQuaternion &a, const CQuaternion &b, float t);
	//----------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------
	float x; ///< Composante X
	float y; ///< Composante Y
	float z; ///< Composante Z
	float w; ///< Composante W
};

//==========================================================
// Fonctions globales relatives aux quaternions
//==========================================================
inline std::istream& operator >>(std::istream& Stream, CQuaternion& Quaternion);
inline std::ostream& operator <<(std::ostream& Stream,
		const CQuaternion& Quaternion);

#include "Quaternion.inl"

} // namespace Math

#endif // QUATERNION_H
