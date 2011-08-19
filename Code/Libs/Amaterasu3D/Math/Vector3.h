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
#include <glm/glm.hpp>

namespace ama3D
{
namespace Math
{

//==========================================================
// Definition des types de vecteurs usuels
//==========================================================
typedef glm::ivec3 TVector3I;
typedef glm::vec3 TVector3F;

} // namespace Math

inline std::ostream & operator<< (std::ostream & s, const ama3D::Math::TVector3I& v)
{
	s << "<" << v.x << "," << v.y << "," << v.z << ">";
	return s;
}

inline std::ostream & operator<< (std::ostream & s, const ama3D::Math::TVector3F& v)
{
	s << "<" << v.x << "," << v.y << "," << v.z << ">";
	return s;
}

}

#endif // VECTOR3_H
