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

#ifndef SINGLETON_H
#define SINGLETON_H

//==========================================================
// En-tetes
//==========================================================
#include <cstdlib>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Template servant a construire les classes singleton
////////////////////////////////////////////////////////////
template<class T>
class CSingleton
{
public:

	//----------------------------------------------------------------
	// Renvoie l'instance unique de la classe
	//----------------------------------------------------------------
	static T& Instance()
	{
		if (!Inst)
			Inst = new T;

		return *Inst;
	}

	//----------------------------------------------------------------
	// D�truit l'instance unique de la classe
	//----------------------------------------------------------------
	static void Destroy()
	{
		delete Inst;
		Inst = NULL;
	}

protected:

	//----------------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------------
	CSingleton()
	{
	}

	//----------------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------------
	~CSingleton()
	{
	}

private:

	//----------------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------------
	static T* Inst; ///< Instance unique de la classe

	//----------------------------------------------------------------
	// Copie interdite
	//----------------------------------------------------------------
	CSingleton(const CSingleton&);
	CSingleton& operator =(const CSingleton&);
};

////////////////////////////////////////////////////////////
/// Macro automatisant la cr�ation d'un singleton --
/// � placer dans la d�finition de la classe concern�e.
/// La raison pour laquelle il faut red�finir (et masquer)
/// la fonction Instance() reste obscure, mais si ce n'est pas
/// fait le client obtient des erreurs de link sur Inst
////////////////////////////////////////////////////////////
#define MAKE_SINGLETON(Class) \
		public : \
		friend class CSingleton<Class>; \
		static Class& Instance(); \
		private :

////////////////////////////////////////////////////////////
/// Macro automatisant
////////////////////////////////////////////////////////////
#define SINGLETON_IMPL(Class) \
		template <> Class* CSingleton<Class>::Inst = NULL; \
		Class& Class::Instance() \
		{ \
			return CSingleton<Class>::Instance(); \
		} \

}

#endif // SINGLETON_H
