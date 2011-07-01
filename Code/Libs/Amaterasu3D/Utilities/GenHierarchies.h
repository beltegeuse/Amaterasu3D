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

#ifndef GENHIERARCHIES_H
#define GENHIERARCHIES_H

//==========================================================
// En-t�tes
//==========================================================
#include <Utilities/Typelist.h>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Hi�rarchie "�parpill�e"
////////////////////////////////////////////////////////////
template<class TList, template<class > class Handler> class CScatteredHierarchy;

template<class T1, class T2, template<class > class Handler>
class CScatteredHierarchy<TypeList<T1, T2> , Handler> : public Handler<T1>,
		public CScatteredHierarchy<T2, Handler>
{
};

template<class T, template<class > class Handler>
class CScatteredHierarchy<TypeList<T, NullType> , Handler> : public Handler<T>
{
};

template<template<class > class Handler>
class CScatteredHierarchy<NullType, Handler>
{
};

////////////////////////////////////////////////////////////
/// Hi�rarchie "lin�aire"
////////////////////////////////////////////////////////////
template<class TList, template<class, class > class Handler
		, class Root = NullType> class CLinearHierarchy;

template<class T1, class T2, template<class, class > class Handler,
class Root>
class CLinearHierarchy<TypeList<T1, T2> , Handler, Root> : public Handler<T1,
		CLinearHierarchy<T2, Handler, Root> >
{
};

template<class T, template<class, class > class Handler,
class Root>
class CLinearHierarchy<TypeList<T, NullType> , Handler, Root> : public Handler<
		T, Root>
{
};

}

#endif // GENHIERARCHIES_H
