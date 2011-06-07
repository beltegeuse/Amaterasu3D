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

#ifndef VECTOR3_H
#define VECTOR3_H

//==========================================================
// En-tetes
//==========================================================
#include <limits>
#include <iostream>
#include <cmath>
#include <tinyxml.h>
#include <TinyXMLHelper.h>

namespace Math
{
    ////////////////////////////////////////////////////////////
    /// Classe des vecteurs e 3 composantes de type parametrable
    ////////////////////////////////////////////////////////////
    template <class T>
    class CVector3
    {
    public :

        //----------------------------------------------------------
        // Constructeur par defaut
        //----------------------------------------------------------
        CVector3(T X = 0, T Y = 0, T Z = 0);

        //----------------------------------------------------------
        // Reinitialise le vecteur
        //----------------------------------------------------------
        void Set(T X, T Y, T Z);

        //----------------------------------------------------------
        // Renvoie la norme du vecteur
        //----------------------------------------------------------
        T Length() const;

        //----------------------------------------------------------
        // Renvoie la norme au carre du vecteur
        //----------------------------------------------------------
        T LengthSq() const;

        //----------------------------------------------------------
        // Normalise le vecteur
        //----------------------------------------------------------
        void Normalize();

        //----------------------------------------------------------
        // Operateurs unaires + et -
        //----------------------------------------------------------
        CVector3<T> operator +() const;
        CVector3<T> operator -() const;

        //----------------------------------------------------------
        // Operateurs binaires + et -
        //----------------------------------------------------------
        CVector3<T> operator +(const CVector3<T>& v) const;
        CVector3<T> operator -(const CVector3<T>& v) const;

        //----------------------------------------------------------
        // Operateurs += et -=
        //----------------------------------------------------------
        const CVector3<T>& operator +=(const CVector3<T>& v);
        const CVector3<T>& operator -=(const CVector3<T>& v);

        //----------------------------------------------------------
        // Operateurs *= et /=
        //----------------------------------------------------------
        const CVector3<T>& operator *=(T t);
        const CVector3<T>& operator /=(T t);

        //----------------------------------------------------------
        // Operateurs de comparaison
        //----------------------------------------------------------
        bool operator ==(const CVector3<T>& v) const;
        bool operator !=(const CVector3<T>& v) const;

        //----------------------------------------------------------
        // Operateur de cast en T*
        //----------------------------------------------------------
        operator T*();

        //----------------------------------------------------------
		// XML operation
		//----------------------------------------------------------
        void WriteXML(TiXmlElement * element);
        void ReadXML(TiXmlElement * element);

        //----------------------------------------------------------
        // Donnees membres
        //----------------------------------------------------------
        T x; ///< Composante X
        T y; ///< Composante Y
        T z; ///< Composante Z
    };


    //==========================================================
    // Fonctions globales relatives aux vecteurs
    //==========================================================
    template <class T> CVector3<T>   operator * (const CVector3<T>& v, T t);
    template <class T> CVector3<T>   operator / (const CVector3<T>& v, T t);
    template <class T> CVector3<T>   operator * (T t, const CVector3<T>& v);
    template <class T> T             operator *  (const CVector3<T>& v1, const CVector3<T>& v2);
    template <class T> CVector3<T>   operator ^(const CVector3<T>& v1, const CVector3<T>& v2);
    template <class T> std::istream& operator >>(std::istream& Stream, CVector3<T>& Vector);
    template <class T> std::ostream& operator <<(std::ostream& Stream, const CVector3<T>& Vector);


    //==========================================================
    // Definition des types de vecteurs usuels
    //==========================================================
    typedef CVector3<int>   TVector3I;
    typedef CVector3<float> TVector3F;

    #include "Vector3.inl"

} // namespace Math


#endif // VECTOR3_H
