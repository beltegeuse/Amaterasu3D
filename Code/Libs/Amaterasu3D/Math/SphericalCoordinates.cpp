//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
// E-mail : adrien.gruson@gmail.com
//==========================================================
#include <Math/SphericalCoordinates.h>
namespace ama3D
{
namespace Math
{

double SphericalCoordinates::Angle(double x, double y)
{
	if (x == 0.0f)
	{
		if (y > 0.0f)
			return M_PI_2;
		return -M_PI_2;
	}
	double res = atan(y / x);
	if (x < 0.0)
	{
		return res + M_PI;
	}
	return res;
}

/** \brief Changement du vecteur repr���sent��� en coordonn���es sph���riques */
void SphericalCoordinates::Set(Math::TVector3F const & v)
{
	m_module = glm::length(v);
	m_phy = acos(v.z / m_module);
	m_theta = Angle(v.x, v.y);
}

/** \brief R���cup���ration du vecteur ��� partir des coordonn���es sph���riques */
Math::TVector3F SphericalCoordinates::Get() const
{
	return Math::TVector3F(m_module * sin(m_phy) * cos(m_theta),
			m_module * sin(m_phy) * sin(m_theta), m_module * cos(m_phy));
}

/** \brief Construction des coordonn���es sph���riques sur la base d'un vecteur 3D */
SphericalCoordinates::SphericalCoordinates(Math::TVector3F const & v)
{
	Set(v);
}

/** \brief Renvoie l'angle entre le vecteur projet��� sur le plan x,y et l'axe X */
double SphericalCoordinates::GetTheta() const
{
	return m_theta;
}

/** \brief Changement de l'angle entre le vecteur projet��� sur le plan x,y et l'axe X */
void SphericalCoordinates::SetTheta(double v)
{
	m_theta = v;
}

/** \brief Renvoie l'angle entre le vecteur et le plan (x,y) */
double SphericalCoordinates::GetPhy() const
{
	return m_phy;
}

/** \brief Changement de l'angle entre le vecteur et le plan (x,y) */
void SphericalCoordinates::SetPhy(double v)
{
	m_phy = v;
}

/** \brief Renvoie le module */
double SphericalCoordinates::GetModule() const
{
	return m_module;
}

/** \brief Changement du module */
void SphericalCoordinates::SetModule(double v)
{
	m_module = v;
}

}
}
