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
/// \param W : Composante W du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector4<T>::CVector4(T X, T Y, T Z, T W) :
x(X),
y(Y),
z(Z),
w(W)
{

}


/////////////////////////////////////////////////////////////
/// R�initialise le vecteur
///
/// \param X : Composante X du vecteur
/// \param Y : Composante Y du vecteur
/// \param Z : Composante Z du vecteur
/// \param W : Composante W du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector4<T>::Set(T X, T Y, T Z, T W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme du vecteur
///
/// \return Valeur de la norme du vecteur sqrt(x� + y� + z� + w�)
///
////////////////////////////////////////////////////////////
#include <math.h>
template <class T>
inline T CVector4<T>::Length() const
{
    return sqrt(LengthSq());
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme au carr� du vecteur
///
/// \return Carr� de la norme du vecteur (x� + y� + z� + w�)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector4<T>::LengthSq() const
{
    return x * x + y * y + z * z + w * w;
}


/////////////////////////////////////////////////////////////
/// Normalise le vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector4<T>::Normalize()
{
    T Norm = Length();

    if (abs(Norm) > std::numeric_limits<T>::epsilon())
    {
        x /= Norm;
        y /= Norm;
        z /= Norm;
        w /= Norm;
    }
}


/////////////////////////////////////////////////////////////
/// Op�rateur unaire +
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector4<T> CVector4<T>::operator +() const
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
inline CVector4<T> CVector4<T>::operator -() const
{
    return CVector4<T>(-x, -y, -z, -w);
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
inline CVector4<T> CVector4<T>::operator +(const CVector4<T>& v) const
{
    return CVector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
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
inline CVector4<T> CVector4<T>::operator -(const CVector4<T>& v) const
{
    return CVector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
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
inline const CVector4<T>& CVector4<T>::operator +=(const CVector4<T>& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

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
inline const CVector4<T>& CVector4<T>::operator -=(const CVector4<T>& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateurs de multiplication avec un scalaire
///
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector4<T> CVector4<T>::operator *(T t) const
{
    return CVector4<T>(x * t, y * t, z * t, w * t);
}


/////////////////////////////////////////////////////////////
/// Op�rateurs de division par un scalaire
///
/// \param t : Scalaire
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector4<T> CVector4<T>::operator /(T t) const
{
    return CVector4<T>(x / t, y / t, z / t, w / t);
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
inline const CVector4<T>& CVector4<T>::operator *=(T t)
{
    x *= t;
    y *= t;
    z *= t;
    w *= t;

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
inline const CVector4<T>& CVector4<T>::operator /=(T t)
{
    x /= t;
    y /= t;
    z /= t;
    w /= t;

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
inline bool CVector4<T>::operator ==(const CVector4<T>& v) const
{
    return ((abs(x - v.x) <= std::numeric_limits<T>::epsilon()) &&
            (abs(y - v.y) <= std::numeric_limits<T>::epsilon()) &&
            (abs(z - v.z) <= std::numeric_limits<T>::epsilon()) &&
            (abs(w - v.w) <= std::numeric_limits<T>::epsilon()));
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
inline bool CVector4<T>::operator !=(const CVector4<T>& v) const
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
inline CVector4<T>::operator T*()
{
    return &x;
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
inline CVector4<T> operator *(const CVector4<T>& v, T t)
{
    return CVector4<T>(v.x * t, v.y * t, v.z * t, v.w * t);
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
inline CVector4<T> operator /(const CVector4<T>& v, T t)
{
    return CVector4<T>(v.x / t, v.y / t, v.z / t, v.w / t);
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
inline CVector4<T> operator *(T t, const CVector4<T>& v)
{
    return v * t;
}


template <class T> T operator *(const CVector4<T>& v1, const CVector4<T>& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
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
template <class T> CVector4<T>   operator ^(const CVector4<T>& v1, const CVector4<T>& v2) 
{
    return CVector4<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.w - v1.w * v2.z, v1.w * v2.x - v1.x * v2.w, v1.x * v2.y - v1.y * v2.x);
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
inline std::istream& operator >>(std::istream& Stream, CVector4<T>& Vector)
{
    return Stream >> Vector.x >> Vector.y >> Vector.z >> Vector.w;
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
inline std::ostream& operator <<(std::ostream& Stream, const CVector4<T>& Vector)
{
    return Stream << Vector.x << " " << Vector.y << " " << Vector.z << " " << Vector.w;
}
