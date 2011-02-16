#ifndef _MATH_SPHERICALCOORDINATES_H_
#define _MATH_SPHERICALCOORDINATES_H_

#include <Math/Vector3.h>
#include <math.h>

namespace Math
{
	/** \brief Classe g�rant une repr�sentation en coordonn�es sph�riques d'un vecteur
	*/
	class SphericalCoordinates
	{
	protected:
		float m_module ;
		float m_phy, m_theta ;

	private:
		/** \brief R�cup�ration de l'angle entre le vecteur (1,0) et le vecteur (x,y) */
		float Angle(float x, float y);

	public:
		/** \brief Changement du vecteur repr�sent� en coordonn�es sph�riques */
		void Set(Math::TVector3F const & v);

		/** \brief R�cup�ration du vecteur � partir des coordonn�es sph�riques */
		Math::TVector3F Get() const;

		/** \brief Construction des coordonn�es sph�riques sur la base d'un vecteur 3D */
		SphericalCoordinates(Math::TVector3F const & v) ;

		/** \brief Renvoie l'angle entre le vecteur projet� sur le plan x,y et l'axe X */
		float GetTheta() const;

		/** \brief Changement de l'angle entre le vecteur projet� sur le plan x,y et l'axe X */
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
