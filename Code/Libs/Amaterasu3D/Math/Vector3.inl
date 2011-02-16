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
/// Constructeur par défaut
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
/// Réinitialise le vecteur
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
/// \return Valeur de la norme du vecteur sqrt(x² + y² + z²)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector3<T>::Length() const
{
    return std::sqrt(LengthSq());
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme au carré du vecteur
///
/// \return Carré de la norme du vecteur (x² + y² + z²)
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
/// Opérateur unaire +
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator +() const
{
    return this;
}


/////////////////////////////////////////////////////////////
/// Opérateur unaire -
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator -() const
{
    return CVector3<T>(-x, -y, -z);
}


/////////////////////////////////////////////////////////////
/// Opérateur binaire +
///
/// \param v : Vecteur à additionner
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator +(const CVector3<T>& v) const
{
    return CVector3<T>(x + v.x, y + v.y, z + v.z);
}


/////////////////////////////////////////////////////////////
/// Opérateur binaire -
///
/// \param v : Vecteur à soustraire
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> CVector3<T>::operator -(const CVector3<T>& v) const
{
    return CVector3<T>(x - v.x, y - v.y, z - v.z);
}

/////////////////////////////////////////////////////////////
/// Opérateur +=
///
/// \param v : Vecteur à additioner
///
/// \return Résultat de l'opération
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
/// Opérateur -=
///
/// \param v : Vecteur à soustraire
///
/// \return Résultat de l'opération
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
/// Opérateur *= avec un scalaire
///
/// \param t : Scalaire
///
/// \return Résultat de l'opération
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
/// Opérateur /= avec un scalaire
///
/// \param t : Scalaire
///
/// \return Résultat de l'opération
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
/// Opérateur de comparaison ==
///
/// \param v : Vecteur à comparer
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
/// Opérateur de comparaison !=
///
/// \param v : Vecteur à comparer
///
/// \return True si les deux vecteurs sont différents
///
////////////////////////////////////////////////////////////
template <class T>
inline bool CVector3<T>::operator !=(const CVector3<T>& v) const
{
    return !(*this == v);
}


/////////////////////////////////////////////////////////////
/// Opérateur de cast en T*
///
/// \return Pointeur sur les composantes du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T>::operator T*()
{
    return &x;
}


/////////////////////////////////////////////////////////////
/// Opérateurs de multiplication avec un scalaire
///
/// \param v : Vecteur
/// \param t : Scalaire
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator *(const CVector3<T>& v, T t)
{
    return CVector3<T>(v.x * t, v.y * t, v.z * t);
}


/////////////////////////////////////////////////////////////
/// Opérateurs de division par un scalaire
///
/// \param v : Vecteur
/// \param t : Scalaire
///
/// \return Résultat de l'opération
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator /(const CVector3<T>& v, T t)
{
    return CVector3<T>(v.x / t, v.y / t, v.z / t);
}


/////////////////////////////////////////////////////////////
/// Opérateurs de multiplication avec un scalaire
///
/// \param t : Scalaire
/// \param v : Vecteur
///
/// \return Résultat de l'opération
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
/// \return Résultat du produit scalaire
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
/// \return Résultat du produit vectoriel
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector3<T> operator ^(const CVector3<T>& v1, const CVector3<T>& v2)
{
    return CVector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'opérateur >> entre un flux et un vecteur
///
/// \param Stream : Flux d'entrée
/// \param Vector : Vecteur
///
/// \return Référence sur le flux d'entrée
///
////////////////////////////////////////////////////////////
template <class T>
inline std::istream& operator >>(std::istream& Stream, CVector3<T>& Vector)
{
    return Stream >> Vector.x >> Vector.y >> Vector.z;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'opérateur << entre un flux et un vecteur
///
/// \param Stream : Flux de sortie
/// \param Vector : Vecteur
///
/// \return Référence sur le flux de sortie
///
////////////////////////////////////////////////////////////
template <class T>
inline std::ostream& operator <<(std::ostream& Stream, const CVector3<T>& Vector)
{
    return Stream << Vector.x << " " << Vector.y << " " << Vector.z;
}
