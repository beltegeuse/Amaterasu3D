
#include <Math/SphericalCoordinates.h>

namespace Math
{

float SphericalCoordinates::Angle(float x, float y)
{
	if(x==0.0f)
	{
		if(y>0.0f) return 3.14159265358979323846f/2.0f ;
		return -3.14159265358979323846f/2.0f ;
	}
	float res = atan(y/x) ;
	if(x<0.0)
	{
		return res+3.14159265358979323846f;
	}
	return res ;
}

/** \brief Changement du vecteur représenté en coordonnées sphériques */
void SphericalCoordinates::Set(Math::TVector3F const & v)
{
	m_module = v.Length() ;
	m_phy = acos(v.z/m_module) ;
	m_theta = Angle(v.x, v.y) ;
}

/** \brief Récupération du vecteur à partir des coordonnées sphériques */
Math::TVector3F SphericalCoordinates::Get() const
{
	return Math::TVector3F(m_module*sin(m_phy)*cos(m_theta),
		m_module*sin(m_phy)*sin(m_theta),
		m_module*cos(m_phy)) ;
}

/** \brief Construction des coordonnées sphériques sur la base d'un vecteur 3D */
SphericalCoordinates::SphericalCoordinates(Math::TVector3F const & v)
{
	Set(v) ;
}

/** \brief Renvoie l'angle entre le vecteur projeté sur le plan x,y et l'axe X */
float SphericalCoordinates::GetTheta() const
{ return m_theta ; }

/** \brief Changement de l'angle entre le vecteur projeté sur le plan x,y et l'axe X */
void SphericalCoordinates::SetTheta(float v)
{ m_theta = v ; }


/** \brief Renvoie l'angle entre le vecteur et le plan (x,y) */
float SphericalCoordinates::GetPhy() const
{ return m_phy ; }

/** \brief Changement de l'angle entre le vecteur et le plan (x,y) */
void SphericalCoordinates::SetPhy(float v)
{ m_phy = v ; }

/** \brief Renvoie le module */
float SphericalCoordinates::GetModule() const
{ return m_module ; }

/** \brief Changement du module */
void SphericalCoordinates::SetModule(float v)
{ m_module = v ; }

};
