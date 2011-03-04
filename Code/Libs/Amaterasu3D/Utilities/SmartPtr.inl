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


////////////////////////////////////////////////////////////
/// Renvoie la donn�e point�e par un smart pointer
///
/// \param Ptr : Pointeur intelligent
///
/// \return Pointeur encapsul� par Ptr
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T*& GetPtr(CSmartPtr<T, Ownership>& Ptr)
{
    return Ptr.m_Data;
}


/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::CSmartPtr() :
m_Data(NULL)
{

}


/////////////////////////////////////////////////////////////
/// Constructeur par copie
///
/// \param Copy : Objet � copier
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::CSmartPtr(const CSmartPtr<T, Ownership>& Copy) :
Ownership<T>(Copy),
m_Data      (Clone(Copy.m_Data))
{

}


/////////////////////////////////////////////////////////////
/// Constructeur � partir d'un pointeur
///
/// \param Pointer : Pointeur � stocker
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::CSmartPtr(T* Pointer) :
m_Data(Pointer)
{

}


/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::~CSmartPtr()
{
    Release(m_Data);
}


/////////////////////////////////////////////////////////////
/// Echange deux CSmartPtr
///
/// \param Ptr : Pointeur � �changer
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline void CSmartPtr<T, Ownership>::Swap(CSmartPtr<T, Ownership>& Ptr)
{
    Ownership<T>::Swap(Ptr);
    std::swap(m_Data, Ptr.m_Data);
}


/////////////////////////////////////////////////////////////
/// Op�rateur de d�r�f�rencement
///
/// \return R�f�rence constante sur la donn�e
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T& CSmartPtr<T, Ownership>::operator *() const
{
    Assert(m_Data != NULL);

    return *m_Data;
}


/////////////////////////////////////////////////////////////
/// Op�rateur d'acc�s
///
/// \return Pointeur constant sur la donn�e
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T* CSmartPtr<T, Ownership>::operator ->() const
{
    Assert(m_Data != NULL);

    return m_Data;
}


/////////////////////////////////////////////////////////////
/// Op�rateur d'affectation
///
/// \param Pointer : Pointeur � affecter
///
/// \return R�f�rence constante sur l'objet courant
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline const CSmartPtr<T, Ownership>& CSmartPtr<T, Ownership>::operator =(const CSmartPtr<T, Ownership>& Pointer)
{
    CSmartPtr<T, Ownership>(Pointer).Swap(*this);

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur d'affectation (� partir d'un pointeur brut).
/// Cette surcharge est n�cessaire pour �viter quelques probl�mes
/// d'auto-affectation
///
/// \param Ptr : Pointeur brut � affecter
///
/// \return R�f�rence constante sur l'objet courant
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline const CSmartPtr<T, Ownership>& CSmartPtr<T, Ownership>::operator =(T* Ptr)
{
    // Si la donn�e stock�e est la m�me que la nouvelle, on ne fait rien !
    // Et sinon, on effectue l'affectation habituelle
    if (m_Data != Ptr)
    {
        CSmartPtr<T, Ownership>(Ptr).Swap(*this);
    }

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op�rateur de cast en T*
///
/// \return Pointeur constant sur la donn�e
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::operator T*() const
{
    return m_Data;
}
