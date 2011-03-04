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

#ifndef UTIL_H
#define UTIL_H

//==========================================================
// En-t�tes
//==========================================================
#include <vector>


/////////////////////////////////////////////////////////////
/// Foncteur servant � faire le m�nage dans les collections
/// de pointeurs
///
////////////////////////////////////////////////////////////
struct Delete
{
	template <class T> void operator ()(T*& p) const
	{
		delete p;
		p = NULL;
	}
};

////////////////////////////////////////////////////////////
/// Renvoie une valeur approch�e d'un nombre � la plus proche
/// plus grande puissance de 2
///
/// \param Value : Nombre � tester
///
/// \return Puissance de 2 correspondante
///
////////////////////////////////////////////////////////////
inline unsigned int NearestPowerOfTwo(unsigned int Value)
{
	unsigned int Temp = Value;
	unsigned int PowerOfTwo = 0;

	while (Temp > 1)
	{
		Temp >>= 1;
		++PowerOfTwo;
	}

	unsigned int Retval = 1 << PowerOfTwo;

	return Retval == Value ? Retval : Retval << 1;
}

#endif // UTIL_H
