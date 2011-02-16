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
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector2<T>::CVector2(T X, T Y) :
x(X),
y(Y)
{

}


/////////////////////////////////////////////////////////////
/// R�initialise le vecteur
///
/// \param X : Composante X du vecteur
/// \param Y : Composante Y du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector2<T>::Set(T X, T Y)
{
    x = X;
    y = Y;
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme du vecteur
///
/// \return Valeur de la norme du vecteur sqrt(x� + y�)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector2<T>::Length() const
{
    return std::sqrt(LengthSq());
}


/////////////////////////////////////////////////////////////
/// Renvoie la norme au carr� du vecteur
///
/// \return Carr� de la norme du vecteur (x� + y�)
///
////////////////////////////////////////////////////////////
template <class T>
inline T CVector2<T>::LengthSq() const
{
    return x * x + y * y;
}


/////////////////////////////////////////////////////////////
/// Normalise le vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CVector2<T>::Normalize()
{
    T Norm = Length();

    if (std::abs(Norm) > std::numeric_limits<T>::epsilon())
    {
        x /= Norm;
        y /= Norm;
    }
}


/////////////////////////////////////////////////////////////
/// Op�rateur unaire +
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector2<T> CVector2<T>::operator +() const
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
inline CVector2<T> CVector2<T>::operator -() const
{
    return CVector2<T>(-x, -y);
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
inline CVector2<T> CVector2<T>::operator +(const CVector2<T>& v) const
{
    return CVector2<T>(x + v.x, y + v.y);
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
inline CVector2<T> CVector2<T>::operator -(const CVector2<T>& v) const
{
    return CVector2<T>(x - v.x, y - v.y);
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
inline const CVector2<T>& CVector2<T>::operator +=(const CVector2<T>& v)
{
    x += v.x;
    y += v.y;

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
inline const CVector2<T>& CVector2<T>::operator -=(const CVector2<T>& v)
{
    x -= v.x;
    y -= v.y;

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
inline CVector2<T> CVector2<T>::operator *(T t) const
{
    return CVector2<T>(x * t, y * t);
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
inline CVector2<T> CVector2<T>::operator /(T t) const
{
    return CVector2<T>(x / t, y / t);
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
inline const CVector2<T>& CVector2<T>::operator *=(T t)
{
    x *= t;
    y *= t;

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
inline const CVector2<T>& CVector2<T>::operator /=(T t)
{
    x /= t;
    y /= t;

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
inline bool CVector2<T>::operator ==(const CVector2<T>& v) const
{
    // FIXME : Verifer la modification
    return ( ( (T) ( std::abs( (double) (x - v.x) ) ) <= std::numeric_limits<T>::epsilon()) && 
           (((T)std::abs((double)(y - v.y)) ) <= std::numeric_limits<T>::epsilon()));
}


/////////////////////////////////////////////////////////////
/// Operateur de comparaison !=
///
/// \param v : Vecteur a comparer
///
/// \return True si les deux vecteurs sont differents
///
////////////////////////////////////////////////////////////
template <class T>
inline bool CVector2<T>::operator !=(const CVector2<T>& v) const
{
    return !(*this == v);
}


/////////////////////////////////////////////////////////////
/// Operateur de cast en T*
///
/// \return Pointeur sur les composantes du vecteur
///
////////////////////////////////////////////////////////////
template <class T>
inline CVector2<T>::operator T*()
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
inline CVector2<T> operator *(const CVector2<T>& v, T t)
{
    return CVector2<T>(v.x * t, v.y * t);
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
inline CVector2<T> operator /(const CVector2<T>& v, T t)
{
    return CVector2<T>(v.x / t, v.y / t);
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
inline CVector2<T> operator *(T t, const CVector2<T>& v)
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
inline T VectorDot(const CVector2<T>& v1, const CVector2<T>& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
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
inline CVector2<T> VectorCross(const CVector2<T>& v1, const CVector2<T>& v2)
{
    return CVector2<T>(/* ??? */);
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
inline std::istream& operator >>(std::istream& Stream, CVector2<T>& Vector)
{
    return Stream >> Vector.x >> Vector.y;
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
inline std::ostream& operator <<(std::ostream& Stream, const CVector2<T>& Vector)
{
    return Stream << Vector.x << " " << Vector.y;
}
