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

#ifndef VECTOR2_H
#define VECTOR2_H

//==========================================================
// En-tetes
//==========================================================
#include <cmath>
#include <limits>
#include <iostream>

namespace ama3D
{
namespace Math
{
////////////////////////////////////////////////////////////
/// Classe des vecteurs a 2 composantes de type paramatrable
////////////////////////////////////////////////////////////
template<class T>
class CVector2
{
public:

	//----------------------------------------------------------
	// Constructeur par dafaut
	//----------------------------------------------------------
	CVector2(T X = 0, T Y = 0);

	//----------------------------------------------------------
	// Rainitialise le vecteur
	//----------------------------------------------------------
	void Set(T X, T Y);

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
	CVector2<T> operator +() const;
	CVector2<T> operator -() const;

	//----------------------------------------------------------
	// Oparateurs binaires + et -
	//----------------------------------------------------------
	CVector2<T> operator +(const CVector2<T>& v) const;
	CVector2<T> operator -(const CVector2<T>& v) const;

	//----------------------------------------------------------
	// Oparateurs += et -=
	//----------------------------------------------------------
	const CVector2<T>& operator +=(const CVector2<T>& v);
	const CVector2<T>& operator -=(const CVector2<T>& v);

	//----------------------------------------------------------
	// Oparateurs * et /
	//----------------------------------------------------------
	CVector2<T> operator *(T t) const;
	CVector2<T> operator /(T t) const;

	//----------------------------------------------------------
	// Oparateurs *= et /=
	//----------------------------------------------------------
	const CVector2<T>& operator *=(T t);
	const CVector2<T>& operator /=(T t);

	//----------------------------------------------------------
	// Oparateurs de comparaison
	//----------------------------------------------------------
	bool operator ==(const CVector2<T>& v) const;
	bool operator !=(const CVector2<T>& v) const;

	//----------------------------------------------------------
	// Oparateur de cast en T*
	//----------------------------------------------------------
	operator T*();

	//----------------------------------------------------------
	// Donnaes membres
	//----------------------------------------------------------
	T x; ///< Composante X
	T y; ///< Composante Y
};

//==========================================================
// Fonctions globales relatives aux vecteurs
//==========================================================
template<class T> CVector2<T> operator *(const CVector2<T>& v, T t);
template<class T> CVector2<T> operator /(const CVector2<T>& v, T t);
template<class T> CVector2<T> operator *(T t, const CVector2<T>& v);
template<class T> T VectorDot(const CVector2<T>& v1, const CVector2<T>& v2);
template<class T> CVector2<T> VectorCross(const CVector2<T>& v1,
		const CVector2<T>& v2);
template<class T> std::istream& operator >>(std::istream& Stream,
		CVector2<T>& Vector);
template<class T> std::ostream& operator <<(std::ostream& Stream,
		const CVector2<T>& Vector);

//==========================================================
// Dafinition des types de vecteurs usuels
//==========================================================
typedef CVector2<int> TVector2I;
typedef CVector2<float> TVector2F;

#include "Vector2.inl"

} // namespace Math
}

#endif // VECTOR2_H
