//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
#ifndef _MATH_SPHERICALCOORDINATES_H_
#define _MATH_SPHERICALCOORDINATES_H_

#include <Math/Vector3.h>
#include <math.h>

namespace Math
{
	/** \brief Classe gérant une représentation en coordonnées sphériques d'un vecteur
	*/
	class SphericalCoordinates
	{
	protected:
		float m_module ;
		float m_phy, m_theta ;

	private:
		/** \brief Récupération de l'angle entre le vecteur (1,0) et le vecteur (x,y) */
		float Angle(float x, float y);

	public:
		/** \brief Changement du vecteur représenté en coordonnées sphériques */
		void Set(Math::TVector3F const & v);

		/** \brief Récupération du vecteur à partir des coordonnées sphériques */
		Math::TVector3F Get() const;

		/** \brief Construction des coordonnées sphériques sur la base d'un vecteur 3D */
		SphericalCoordinates(Math::TVector3F const & v) ;

		/** \brief Renvoie l'angle entre le vecteur projeté sur le plan x,y et l'axe X */
		float GetTheta() const;

		/** \brief Changement de l'angle entre le vecteur projeté sur le plan x,y et l'axe X */
		void SetTheta(float v);


		/** \brief Renvoie l'angle entre le vecteur et le plan (x,y) */
		float GetPhy() const;

		/** \brief Changement de l'angle entre le vecteur et le plan (x,y) */
		void SetPhy(float v);

		/** \brief Renvoie le module */
		float GetModule() const;

		/** \brief Changement du module */
		void SetModule(float v);
	} ;
}

#endif /*SPHERICALCOORDINATES_H_*/
