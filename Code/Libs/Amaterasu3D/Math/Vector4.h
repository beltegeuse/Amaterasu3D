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

#ifndef VECTOR4_H
#define VECTOR4_H

//==========================================================
// En-tates
//==========================================================
#include <limits>
#include <iostream>

namespace Math
{
////////////////////////////////////////////////////////////
/// Classe des vecteurs a 4 composantes de type paramatrable
////////////////////////////////////////////////////////////
template<class T>
class CVector4
{
public:

	//----------------------------------------------------------
	// Constructeur par dafaut
	//----------------------------------------------------------
	CVector4(T X = 0, T Y = 0, T Z = 0, T W = 1);

	//----------------------------------------------------------
	// Rainitialise le vecteur
	//----------------------------------------------------------
	void Set(T X, T Y, T Z, T W);

	//----------------------------------------------------------
	// Renvoie la norme du vecteur
	//----------------------------------------------------------
	T Length() const;

	//----------------------------------------------------------
	// Renvoie la norme au carra du vecteur
	//----------------------------------------------------------
	T LengthSq() const;

	//----------------------------------------------------------
	// Normalise le vecteur
	//----------------------------------------------------------
	void Normalize();

	//----------------------------------------------------------
	// Oparateurs unaires + et -
	//----------------------------------------------------------
	CVector4<T> operator +() const;
	CVector4<T> operator -() const;

	//----------------------------------------------------------
	// Oparateurs binaires + et -
	//----------------------------------------------------------
	CVector4<T> operator +(const CVector4<T>& v) const;
	CVector4<T> operator -(const CVector4<T>& v) const;

	//----------------------------------------------------------
	// Oparateurs += et -=
	//----------------------------------------------------------
	const CVector4<T>& operator +=(const CVector4<T>& v);
	const CVector4<T>& operator -=(const CVector4<T>& v);

	//----------------------------------------------------------
	// Oparateurs * et /
	//----------------------------------------------------------
	CVector4<T> operator *(T t) const;
	CVector4<T> operator /(T t) const;

	//----------------------------------------------------------
	// Oparateurs *= et /=
	//----------------------------------------------------------
	const CVector4<T>& operator *=(T t);
	const CVector4<T>& operator /=(T t);

	//----------------------------------------------------------
	// Oparateurs de comparaison
	//----------------------------------------------------------
	bool operator ==(const CVector4<T>& v) const;
	bool operator !=(const CVector4<T>& v) const;

	//----------------------------------------------------------
	// Oparateur de cast en T*
	//----------------------------------------------------------
	operator T*();

	//----------------------------------------------------------
	// Donnaes membres
	//----------------------------------------------------------
	T x; ///< Composante X
	T y; ///< Composante Y
	T z; ///< Composante Z
	T w; ///< Composante W
};

//==========================================================
// Fonctions globales relatives aux vecteurs
//==========================================================
template<class T> CVector4<T> operator *(const CVector4<T>& v, T t);
template<class T> CVector4<T> operator /(const CVector4<T>& v, T t);
template<class T> CVector4<T> operator *(T t, const CVector4<T>& v);
template<class T> CVector4<T> operator *(const CVector4<T>& v1,
		const CVector4<T>& v2);
template<class T> CVector4<T> operator ^(const CVector4<T>& v1,
		const CVector4<T>& v2);
template<class T> std::istream& operator >>(std::istream& Stream,
		CVector4<T>& Vector);
template<class T> std::ostream& operator <<(std::ostream& Stream,
		const CVector4<T>& Vector);

//==========================================================
// Definition des types de vecteurs usuels
//==========================================================
typedef CVector4<int> TVector4I;
typedef CVector4<float> TVector4F;

#include "Vector4.inl"

} // namespace Math

#endif // VECTOR4_H
