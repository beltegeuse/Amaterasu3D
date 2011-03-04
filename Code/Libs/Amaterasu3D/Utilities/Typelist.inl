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
// Recherche le N-ième élément d'une typelist
////////////////////////////////////////////////////////////
template <std::size_t N, class List>
struct NthElement
{
    typedef typename NthElement<N - 1, typename List::Tail>::Result Result;
};

template <class List>
struct NthElement<0, List>
{
    typedef typename List::Head Result;
};


////////////////////////////////////////////////////////////
// Détermine de la taille d'une typelist
////////////////////////////////////////////////////////////
template <class List>
struct Size
{
    enum {Result = Size<typename List::Tail>::Result + 1};
};

template <>
struct Size<NullType>
{
    enum {Result = 0};
};


////////////////////////////////////////////////////////////
// Recherche la position d'un élément dans une typelist
////////////////////////////////////////////////////////////
template <class ToFind, class ToCompare, class List>
struct Find_
{
    enum {Result = Find<ToFind, List>::Result + 1};
};

template <class ToFind, class List>
struct Find_<ToFind, ToFind, List>
{
    enum {Result = 0};
};

template <class ToFind, class List>
struct Find
{
    enum {Result = Find_<ToFind, typename List::Head, typename List::Tail>::Result};
};


////////////////////////////////////////////////////////////
// Ajoute un élément à la fin d'une typelist
////////////////////////////////////////////////////////////
template <class T, class List>
struct PushBack
{
    typedef TypeList<typename List::Head, typename PushBack<T, typename List::Tail>::Result> Result;
};

template <class T>
struct PushBack<T, NullType>
{
    typedef TypeList<T, NullType> Result;
};


////////////////////////////////////////////////////////////
// Ajoute d'un élément au début d'une typelist
////////////////////////////////////////////////////////////
template <class T, class List>
struct PushFront
{
    typedef TypeList<T, List> Result;
};


////////////////////////////////////////////////////////////
// Concatène 2 typelists
////////////////////////////////////////////////////////////
template <class List1, class List2>
struct Concat
{
    typedef typename Concat<typename PushBack<typename List2::Head, List1>::Result, typename List2::Tail>::Result Result;
};

template <class List1>
struct Concat<List1, NullType>
{
    typedef List1 Result;
};
