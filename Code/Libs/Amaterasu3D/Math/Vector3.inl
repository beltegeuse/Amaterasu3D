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


/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
/// \param X : Composante X du vecteur
/// \param Y : Composante Y du vecteur
/// \param Z : Composante Z du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T>::CVector3(T X, T Y, T Z) :
x(X),
y(Y),
z(Z)
{

}


/////////////////////////////////////////////////////////////
/// R�initialise le vecteur
///
/// \param X : Composante X du vecteur
/// \param Y : Composante Y du vecteur
/// \param Z : Composante Z du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector3<T>::Set(T X, T Y, T Z)
{
    x = X;
    y = Y;
    z = Z;
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme du vecteur
///
/// \return Valeur de la norme du vecteur sqrt(x� + y� + z�)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector3<T>::Length() const
{
    return std::sqrt(LengthSq());
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme au carr� du vecteur
///
/// \return Carr� de la norme du vecteur (x� + y� + z�)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector3<T>::LengthSq() const
{
    return x * x + y * y + z * z;
}


/////////////////////////////////////////////////////////////
/// Normalise le vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector3<T>::Normalize()
{
    T Norm = Length();

    if (std::abs(Norm) > std::numeric_limits<T>::epsilon())
    {
        x /= Norm;
        y /= Norm;
        z /= Norm;
    }
}


/////////////////////////////////////////////////////////////
/// Op�rateur unaire +
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator +() const
{
    return this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur unaire -
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator -() const
{
    return CVector3<T>(-x, -y, -z);
}


/////////////////////////////////////////////////////////////
/// Op�rateur binaire +
///
/// \param v : Vecteur � additionner
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator +(const CVector3<T>& v) const
{
    return CVector3<T>(x + v.x, y + v.y, z + v.z);
}


/////////////////////////////////////////////////////////////
/// Op�rateur binaire -
///
/// \param v : Vecteur � soustraire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator -(const CVector3<T>& v) const
{
    return CVector3<T>(x - v.x, y - v.y, z - v.z);
}

/////////////////////////////////////////////////////////////
/// Op�rateur +=
///
/// \param v : Vecteur � additioner
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline const CVector3<T>& CVector3<T>::operator +=(const CVector3<T>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur -=
///
/// \param v : Vecteur � soustraire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline const CVector3<T>& CVector3<T>::operator -=(const CVector3<T>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur *= avec un scalaire
///
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline const CVector3<T>& CVector3<T>::operator *=(T t)
{
    x *= t;
    y *= t;
    z *= t;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur /= avec un scalaire
///
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline const CVector3<T>& CVector3<T>::operator /=(T t)
{
    x /= t;
    y /= t;
    z /= t;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur de comparaison ==
///
/// \param v : Vecteur � comparer
///
/// \return True si les deux vecteurs sont identiques
///
////////////////////////////////////////////////////////////
template <class T>
inline bool CVector3<T>::operator ==(const CVector3<T>& v) const
{
    return ((std::abs(x - v.x) <= std::numeric_limits<T>::epsilon()) &&
            (std::abs(y - v.y) <= std::numeric_limits<T>::epsilon()) &&
            (std::abs(z - v.z) <= std::numeric_limits<T>::epsilon()));
}


/////////////////////////////////////////////////////////////
/// Op�rateur de comparaison !=
///
/// \param v : Vecteur � comparer
///
/// \return True si les deux vecteurs sont diff�rents
///
////////////////////////////////////////////////////////////
template <class T>
inline bool CVector3<T>::operator !=(const CVector3<T>& v) const
{
    return !(*this == v);
}


/////////////////////////////////////////////////////////////
/// Op�rateur de cast en T*
///
/// \return Pointeur sur les composantes du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T>::operator T*()
{
    return &x;
}
#include <iostream>

template <class T>
inline void CVector3<T>::WriteXML(TiXmlElement * element)
{
	element->SetAttribute("x", x);
	element->SetAttribute("y", y);
	element->SetAttribute("z", z);
}

// FIXME: Tricks to avoid presicion problem in tinyxml
template <>
inline void CVector3<float>::WriteXML(TiXmlElement * element)
{
	// Copy to double to keep precision
	double vX = x;
	double vY = y;
	double vZ = z; 
	
	element->SetAttribute("x", vX);
	element->SetAttribute("y", vY);
	element->SetAttribute("z", vZ);
}

template <class T>
inline void CVector3<T>::ReadXML(TiXmlElement * element)
{
	TinyXMLGetAttributeValue(element, "x", &x);
	TinyXMLGetAttributeValue(element, "y", &y);
	TinyXMLGetAttributeValue(element, "z", &z);
}

// FIXME: Tricks to avoid presicion problem in tinyxml
template <>
inline void CVector3<float>::ReadXML(TiXmlElement * element)
{
	// Copy to double to keep precision
	double vX;
	double vY;
	double vZ; 

	TinyXMLGetAttributeValue(element, "x", &vX);
	TinyXMLGetAttributeValue(element, "y", &vY);
	TinyXMLGetAttributeValue(element, "z", &vZ);
	
	x = vX;
	y = vY;
	z = vZ;
}


/////////////////////////////////////////////////////////////
/// Op�rateurs de multiplication avec un scalaire
///
/// \param v : Vecteur
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator *(const CVector3<T>& v, T t)
{
    return CVector3<T>(v.x * t, v.y * t, v.z * t);
}


/////////////////////////////////////////////////////////////
/// Op�rateurs de division par un scalaire
///
/// \param v : Vecteur
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator /(const CVector3<T>& v, T t)
{
    return CVector3<T>(v.x / t, v.y / t, v.z / t);
}


/////////////////////////////////////////////////////////////
/// Op�rateurs de multiplication avec un scalaire
///
/// \param t : Scalaire
/// \param v : Vecteur
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator *(T t, const CVector3<T>& v)
{
    return v * t;
}


/////////////////////////////////////////////////////////////
/// Effectue le produit scalaire entre deux vecteurs
///
/// \param v1 : Vecteur 1
/// \param v2 : Vecteur 2
///
/// \return R�sultat du produit scalaire
///
////////////////////////////////////////////////////////////
template <class T>
inline T operator *(const CVector3<T>& v1, const CVector3<T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


/////////////////////////////////////////////////////////////
/// Effectue le produit vectoriel entre deux vecteurs
///
/// \param v1 : Vecteur 1
/// \param v2 : Vecteur 2
///
/// \return R�sultat du produit vectoriel
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator ^(const CVector3<T>& v1, const CVector3<T>& v2)
{
    return CVector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op�rateur >> entre un flux et un vecteur
///
/// \param Stream : Flux d'entr�e
/// \param Vector : Vecteur
///
/// \return R�f�rence sur le flux d'entr�e
///
////////////////////////////////////////////////////////////
template <class T>
inline std::istream& operator >>(std::istream& Stream, CVector3<T>& Vector)
{
    return Stream >> Vector.x >> Vector.y >> Vector.z;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op�rateur << entre un flux et un vecteur
///
/// \param Stream : Flux de sortie
/// \param Vector : Vecteur
///
/// \return R�f�rence sur le flux de sortie
///
////////////////////////////////////////////////////////////
template <class T>
inline std::ostream& operator <<(std::ostream& Stream, const CVector3<T>& Vector)
{
    return Stream << Vector.x << " " << Vector.y << " " << Vector.z;
}
