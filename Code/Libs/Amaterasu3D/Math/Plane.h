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

#ifndef PLANE_H
#define PLANE_H

//==========================================================
// En-t�tes
//==========================================================
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace Math
{
////////////////////////////////////////////////////////////
/// Plans � 4 composantes r�elles
////////////////////////////////////////////////////////////
class CPlane
{
public:

	//----------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------
	CPlane(float A = 0.0f, float B = 0.0f, float C = 0.0f, float D = 0.0f);

	//----------------------------------------------------------
	// Constructeur � partir de trois points
	//----------------------------------------------------------
	CPlane(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2);

	//----------------------------------------------------------
	// Constructeur � partir d'une normale et d'un point
	//----------------------------------------------------------
	CPlane(const TVector3F& Normal, const TVector3F& Point);

	//----------------------------------------------------------
	// Construit le plan � partir de trois points
	//----------------------------------------------------------
	void BuildFromPoints(const TVector3F& v0, const TVector3F& v1,
			const TVector3F& v2);

	//----------------------------------------------------------
	// Renvoie la plus courte distance entre le plan et un point
	//----------------------------------------------------------
	float DistanceToPoint(const TVector3F& Point) const;
	float DistanceToPoint(const TVector4F& Point) const;

	//----------------------------------------------------------
	// Op�rateurs de comparaison
	//----------------------------------------------------------
	bool operator ==(const CPlane& p) const;
	bool operator !=(const CPlane& p) const;

	//----------------------------------------------------------
	// Intersections
	//----------------------------------------------------------

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	TVector3F n; ///< Normale du plan
	float d; ///< Distance du plan � l'originie
};

//==========================================================
// Fonctions globales relatives aux plan
//==========================================================
std::istream& operator >>(std::istream& Stream, CPlane& Plane);
std::ostream& operator <<(std::ostream& Stream, const CPlane& Plane);

#include "Plane.inl"

}

#endif // PLANE_H
