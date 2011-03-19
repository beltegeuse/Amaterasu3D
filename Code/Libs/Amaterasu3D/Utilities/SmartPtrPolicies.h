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

#ifndef SMARTPTRPOLICIES_H
#define SMARTPTRPOLICIES_H

#include <algorithm>

//==========================================================
// En-têtes
//==========================================================


////////////////////////////////////////////////////////////
/// Police de pointeur intelligent :
/// comptage de référence externe
////////////////////////////////////////////////////////////
template <class T>
class CRefCount
{
public :

	//----------------------------------------------------------
	// Constructeur par défaut
	//----------------------------------------------------------
	CRefCount() : m_Counter(new int(1))
	{
	}

	//----------------------------------------------------------
	// Clone la ressource
	//----------------------------------------------------------
	T* Clone(T* Ptr)
	{
		++*m_Counter;
		return Ptr;
	}

	//----------------------------------------------------------
	// Gère la libération de la ressource
	//----------------------------------------------------------
	void Release(T* Ptr)
	{
		if (--*m_Counter == 0)
		{
			delete m_Counter;
			delete Ptr;
		}
	}

	//----------------------------------------------------------
	// Echange deux instances
	//----------------------------------------------------------
	void Swap(CRefCount& RefCount)
	{
		std::swap(RefCount.m_Counter, m_Counter);
	}

private :

	int* m_Counter; ///< Pointeur sur le compteur de référence
};

////////////////////////////////////////////////////////////
/// Police de pointeur intelligent :
/// objets COM et ressources du moteur - comptage de référence intrusif
////////////////////////////////////////////////////////////
template <class T>
class CResourceCOM
{
public :

	//----------------------------------------------------------
	// Clone la ressource
	//----------------------------------------------------------
	static T* Clone(T* Ptr)
	{
		if (Ptr)
			Ptr->AddRef();
		return Ptr;
	}

	//----------------------------------------------------------
	// Gère la libération de la ressource
	//----------------------------------------------------------
	static void Release(T* Ptr)
	{
		if (Ptr)
			Ptr->Release();
	}

	//----------------------------------------------------------
	// Echange deux instances - aucune donnée membre : ne fait rien
	//----------------------------------------------------------
	static void Swap(CResourceCOM&)
	{
	}
};



#endif // SMARTPTRPOLICIES_H
