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
/// Renvoie la donnée pointée par un smart pointer
///
/// \param Ptr : Pointeur intelligent
///
/// \return Pointeur encapsulé par Ptr
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T*& GetPtr(CSmartPtr<T, Ownership>& Ptr)
{
    return Ptr.m_Data;
}


/////////////////////////////////////////////////////////////
/// Constructeur par défaut
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
/// \param Copy : Objet à copier
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::CSmartPtr(const CSmartPtr<T, Ownership>& Copy) :
Ownership<T>(Copy),
m_Data      (Clone(Copy.m_Data))
{

}


/////////////////////////////////////////////////////////////
/// Constructeur à partir d'un pointeur
///
/// \param Pointer : Pointeur à stocker
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
/// \param Ptr : Pointeur à échanger
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline void CSmartPtr<T, Ownership>::Swap(CSmartPtr<T, Ownership>& Ptr)
{
    Ownership<T>::Swap(Ptr);
    std::swap(m_Data, Ptr.m_Data);
}


/////////////////////////////////////////////////////////////
/// Opérateur de déréférencement
///
/// \return Référence constante sur la donnée
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T& CSmartPtr<T, Ownership>::operator *() const
{
    Assert(m_Data != NULL);

    return *m_Data;
}


/////////////////////////////////////////////////////////////
/// Opérateur d'accès
///
/// \return Pointeur constant sur la donnée
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline T* CSmartPtr<T, Ownership>::operator ->() const
{
    Assert(m_Data != NULL);

    return m_Data;
}


/////////////////////////////////////////////////////////////
/// Opérateur d'affectation
///
/// \param Pointer : Pointeur à affecter
///
/// \return Référence constante sur l'objet courant
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline const CSmartPtr<T, Ownership>& CSmartPtr<T, Ownership>::operator =(const CSmartPtr<T, Ownership>& Pointer)
{
    CSmartPtr<T, Ownership>(Pointer).Swap(*this);

    return *this;
}


/////////////////////////////////////////////////////////////
/// Opérateur d'affectation (à partir d'un pointeur brut).
/// Cette surcharge est nécessaire pour éviter quelques problèmes
/// d'auto-affectation
///
/// \param Ptr : Pointeur brut à affecter
///
/// \return Référence constante sur l'objet courant
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline const CSmartPtr<T, Ownership>& CSmartPtr<T, Ownership>::operator =(T* Ptr)
{
    // Si la donnée stockée est la même que la nouvelle, on ne fait rien !
    // Et sinon, on effectue l'affectation habituelle
    if (m_Data != Ptr)
    {
        CSmartPtr<T, Ownership>(Ptr).Swap(*this);
    }

    return *this;
}


/////////////////////////////////////////////////////////////
/// Opérateur de cast en T*
///
/// \return Pointeur constant sur la donnée
///
////////////////////////////////////////////////////////////
template <class T, template <class> class Ownership>
inline CSmartPtr<T, Ownership>::operator T*() const
{
    return m_Data;
}
