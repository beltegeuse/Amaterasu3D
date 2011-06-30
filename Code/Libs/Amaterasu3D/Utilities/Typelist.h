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

#ifndef TYPELIST_H
#define TYPELIST_H

////////////////////////////////////////////////////////////
/// Structure d'une typelist (liste de types)
////////////////////////////////////////////////////////////
template<class T1, class T2>
struct TypeList
{
	typedef T1 Head;
	typedef T2 Tail;
};

////////////////////////////////////////////////////////////
/// Marqueur de fin de liste
////////////////////////////////////////////////////////////
struct NullType
{
};

////////////////////////////////////////////////////////////
// Macros servant � construire des typelists
////////////////////////////////////////////////////////////
#define TYPELIST_1(t1)                                 TypeList<t1, NullType>
#define TYPELIST_2(t1, t2)                             TypeList<t1, TYPELIST_1(t2) >
#define TYPELIST_3(t1, t2, t3)                         TypeList<t1, TYPELIST_2(t2, t3) >
#define TYPELIST_4(t1, t2, t3, t4)                     TypeList<t1, TYPELIST_3(t2, t3, t4) >
#define TYPELIST_5(t1, t2, t3, t4, t5)                 TypeList<t1, TYPELIST_4(t2, t3, t4, t5) >
#define TYPELIST_6(t1, t2, t3, t4, t5, t6)             TypeList<t1, TYPELIST_5(t2, t3, t4, t5, t6) >
#define TYPELIST_7(t1, t2, t3, t4, t5, t6, t7)         TypeList<t1, TYPELIST_6(t2, t3, t4, t5, t6, t7) >
#define TYPELIST_8(t1, t2, t3, t4, t5, t6, t7, t8)     TypeList<t1, TYPELIST_7(t2, t3, t4, t5, t6, t7, t8) >
#define TYPELIST_9(t1, t2, t3, t4, t5, t6, t7, t8, t9) TypeList<t1, TYPELIST_8(t2, t3, t4, t5, t6, t7, t8, t9) >

////////////////////////////////////////////////////////////
// Quelques op�rations utiles sur les typelists
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Recherche le N-i�me �l�ment d'une typelist
////////////////////////////////////////////////////////////
template<std::size_t N, class List> struct NthElement;

////////////////////////////////////////////////////////////
/// D�termine de la taille d'une typelist
////////////////////////////////////////////////////////////
template<class List> struct Size;

////////////////////////////////////////////////////////////
/// Recherche la position d'un �l�ment dans une typelist
////////////////////////////////////////////////////////////
template<class ToFind, class List> struct Find;

////////////////////////////////////////////////////////////
/// Ajoute un �l�ment � la fin d'une typelist
////////////////////////////////////////////////////////////
template<class T, class List> struct PushBack;

////////////////////////////////////////////////////////////
/// Ajoute d'un �l�ment au d�but d'une typelist
////////////////////////////////////////////////////////////
template<class T, class List> struct PushFront;

////////////////////////////////////////////////////////////
/// Concat�ne 2 typelists
////////////////////////////////////////////////////////////
template<class List1, class List2> struct Concat;

#include "Typelist.inl"

#endif // TYPELIST_H
