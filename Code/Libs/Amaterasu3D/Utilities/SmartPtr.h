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

#ifndef SMARTPTR_H
#define SMARTPTR_H

//==========================================================
// En-t�tes
//==========================================================
#include <cstdlib>
#include <Utilities/SmartPtrPolicies.h>
#include <Debug/Exceptions.h>

////////////////////////////////////////////////////////////
// D�clarations anticip�es
////////////////////////////////////////////////////////////
template<class, template<class > class> class CSmartPtr;
template<class T, template<class > class Ownership> T*& GetPtr(
		CSmartPtr<T, Ownership>& Ptr);

////////////////////////////////////////////////////////////
/// Pointeurs intelligents � base de polices
////////////////////////////////////////////////////////////
template<class T, template<class > class Ownership = CRefCount>
class CSmartPtr: public Ownership<T>
{
public:

	//----------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------
	CSmartPtr();

	//----------------------------------------------------------
	// Constructeur par copie
	//----------------------------------------------------------
	CSmartPtr(const CSmartPtr& Copy);

	//----------------------------------------------------------
	// Constructeur � partir d'un pointeur
	//----------------------------------------------------------
	CSmartPtr(T* Pointer);

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	~CSmartPtr();

	//----------------------------------------------------------
	// Op�rateurs de d�r�f�rencement
	//----------------------------------------------------------
	T& operator *() const;
	T* operator ->() const;

	//----------------------------------------------------------
	// Op�rateur d'affectation
	//----------------------------------------------------------
	const CSmartPtr& operator =(const CSmartPtr& Pointer);

	//----------------------------------------------------------
	// Op�rateur d'affectation (� partir d'un pointeur brut)
	//----------------------------------------------------------
	const CSmartPtr& operator =(T* Ptr);

	//----------------------------------------------------------
	// Op�rateur de cast en T*
	//----------------------------------------------------------
	operator T*() const;

private:

	//----------------------------------------------------------
	// Amis
	//----------------------------------------------------------
	friend T*& GetPtr<T, Ownership>(CSmartPtr<T, Ownership>& Ptr);

	//----------------------------------------------------------
	// Echange deux CSmartPtr
	//----------------------------------------------------------
	void Swap(CSmartPtr& Ptr);

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	T* m_Data; ///< Pointeur vers la donn�es stock�e
};

#include "SmartPtr.inl"

#endif // SMARTPTR_H
