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


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
/// \param X : Composante X
/// \param Y : Composante Y
/// \param Z : Composante Z
/// \param W : Composante W
///
////////////////////////////////////////////////////////////
inline CQuaternion::CQuaternion(float X, float Y, float Z, float W) :
x(X),
y(Y),
z(Z),
w(W)
{

}


/////////////////////////////////////////////////////////////
/// Constructeur � partir d'une matrice
///
/// \param Matrix : Matrice source
///
////////////////////////////////////////////////////////////
inline CQuaternion::CQuaternion(const CMatrix4& Matrix)
{
    FromMatrix(Matrix);
}


/////////////////////////////////////////////////////////////
/// Constructeur � partir d'un axe et d'un angle
///
/// \param Axis :  Axe de rotation
/// \param Angle : Angle de rotation
///
////////////////////////////////////////////////////////////
inline CQuaternion::CQuaternion(const TVector3F& Axis, float Angle)
{
    FromAxisAngle(Axis, Angle);
}


/////////////////////////////////////////////////////////////
/// Met le quaternion � l'identit�
///
////////////////////////////////////////////////////////////
inline void CQuaternion::Identity()
{
    x = y = z = 0.0f;
    w = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Normalise le quaternion
///
////////////////////////////////////////////////////////////
inline void CQuaternion::Normalize()
{
    float Norm = x * x + y * y + z * z + w * w;

    if (std::fabs(Norm) > std::numeric_limits<float>::epsilon())
    {
        x /= Norm;
        y /= Norm;
        z /= Norm;
        w /= Norm;
    }
}


/////////////////////////////////////////////////////////////
/// Renvoie le conjugu�
///
/// \return Quanternion conjugu�
///
////////////////////////////////////////////////////////////
inline CQuaternion CQuaternion::Conjugate() const
{
    return CQuaternion(-x, -y, -z, w);
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation � partir du quaternion
///
/// \return Matrice construite
///
////////////////////////////////////////////////////////////
inline CMatrix4 CQuaternion::ToMatrix() const
{
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;
    float yy = y * y;
    float yz = y * z;
    float yw = y * w;
    float zz = z * z;
    float zw = z * w;

    return CMatrix4(1 - 2 * (yy + zz),     2 * (xy - zw),     2 * (xy + yw), 0,
                        2 * (xy + zw), 1 - 2 * (xx + zz),     2 * (yz - xw), 0,
                        2 * (xz - yw),     2 * (yz + xw), 1 - 2 * (xx + yy), 0,
                                    0,                 0,                 0, 1);
}


/////////////////////////////////////////////////////////////
/// Construit le quaternion � partir d'une matrice de rotation
///
/// \param Matrix : Matrice source
///
////////////////////////////////////////////////////////////
inline void CQuaternion::FromMatrix(const CMatrix4& Matrix)
{
    float Trace = Matrix(0, 0) + Matrix(1, 1) + Matrix(2, 2) + 1;

    if (Trace > 0)
    {
        float s = 0.5f / std::sqrt(Trace);
        x = (Matrix(2, 1) - Matrix(1, 2)) * s;
        y = (Matrix(0, 2) - Matrix(2, 0)) * s;
        z = (Matrix(1, 0) - Matrix(0, 1)) * s;
        w = 0.25f / s;
    }
    else
    {
        if ((Matrix(0, 0) > Matrix(1, 1) && (Matrix(0, 0) > Matrix(2, 2))))
        {
            float s = std::sqrt(1 + Matrix(0, 0) - Matrix(1, 1) - Matrix(2, 2)) * 2;
            x = 0.5f / s;
            y = (Matrix(0, 1) + Matrix(1, 0)) / s;
            z = (Matrix(0, 2) + Matrix(2, 0)) / s;
            w = (Matrix(1, 2) + Matrix(2, 1)) / s;
        }
        else if (Matrix(1, 1) > Matrix(2, 2))
        {
            float s = std::sqrt(1 - Matrix(0, 0) + Matrix(1, 1) - Matrix(2, 2)) * 2;
            x = (Matrix(0, 1) + Matrix(1, 0)) / s;
            y = 0.5f / s;
            z = (Matrix(1, 2) + Matrix(2, 1)) / s;
            w = (Matrix(0, 2) + Matrix(2, 0)) / s;
        }
        else
        {
            float s = std::sqrt(1 - Matrix(0, 0) - Matrix(1, 1) + Matrix(2, 2)) * 2;
            x = (Matrix(0, 2) + Matrix(2, 0)) / s;
            y = (Matrix(1, 2) + Matrix(2, 1)) / s;
            z = 0.5f / s;
            w = (Matrix(0, 1) + Matrix(1, 0)) / s;
        }
    }
}


/////////////////////////////////////////////////////////////
/// Construit le quaternion � partir d'un axe et d'un angle
///
/// \param Axis :  Axe de rotation
/// \param Angle : Angle
///
////////////////////////////////////////////////////////////
inline void CQuaternion::FromAxisAngle(const TVector3F& Axis, float Angle)
{
    float Cos = std::cos(Angle / 2);
    float Sin = std::sin(Angle / 2);

    x = Axis.x * Sin;
    y = Axis.y * Sin;
    z = Axis.z * Sin;
    w = Cos;

    Normalize();
}


/////////////////////////////////////////////////////////////
/// Transforme le quaternion en axe & angle de rotation
///
/// \param Axis :  Axe de rotation
/// \param Angle : Angle de rotation
///
////////////////////////////////////////////////////////////
inline void CQuaternion::ToAxisAngle(TVector3F& Axis, float& Angle) const
{
    // Calcul de l'angle
    Angle = std::acos(w) * 2;

    // Calcul de l'axe
    float Norm = std::sqrt(x * x + y * y + z * z);
    if (std::fabs(Norm) > std::numeric_limits<float>::epsilon())
    {
        Axis.x = x / Norm;
        Axis.y = y / Norm;
        Axis.z = z / Norm;
    }
    else
    {
        Axis.x = 0.0f;
        Axis.y = 1.0f;
        Axis.z = 0.0f;
    }
}


/////////////////////////////////////////////////////////////
/// Construit le quaternion � partir de 3 angles d'Euler
///
/// \param X : Angle autour de X
/// \param Y : Angle autour de Y
/// \param Z : Angle autour de Z
///
////////////////////////////////////////////////////////////
inline void CQuaternion::FromEulerAngles(float X, float Y, float Z)
{
    CQuaternion Qx(TVector3F(1, 0, 0), X);
    CQuaternion Qy(TVector3F(0, 1, 0), Y);
    CQuaternion Qz(TVector3F(0, 0, 1), Z);

    *this = Qx * Qy * Qz;
}

#include <iostream>

inline void CQuaternion::From3DVector(const Math::TVector3F& v)
{
	//std::cout << std::endl << "DEBUG : " << v << std::endl;
	SphericalCoordinates s(v);
	//std::cout << "DEBUG : " << -s.GetTheta() << " | " << -s.GetPhy()+M_PI/2.0 << std::endl;
	CQuaternion Qx(TVector3F(1, 0, 0), 0);
	CQuaternion Qz(TVector3F(0, 0, 1), -s.GetTheta());
	CQuaternion Qy(TVector3F(0, 1, 0), -s.GetPhy()+M_PI_2);
	
	//std::cout << "Qy : " << Qy << std::endl;
	//std::cout << "Qz : " << Qz << std::endl;
	
	*this = Qx*Qy*Qz;
	Normalize();
	
	//std::cout << "Matrix : ======" << std::endl;
	//std::cout << ToMatrix() << std::endl;
	//std::cout << "Matrix : ======" << std::endl;
	//std::cout << "Quaternion : " << *this << std::endl;
}


/////////////////////////////////////////////////////////////
/// Op�rateur de multiplication
///
/// \param Quaternion : Quaternion � multiplier
///
/// \return R�sultat de l'op�ration
///
////////////////////////////////////////////////////////////
inline CQuaternion CQuaternion::operator *(const CQuaternion& Quaternion) const
{
    return CQuaternion(w * Quaternion.x + x * Quaternion.w + y * Quaternion.z - z * Quaternion.y,
                       w * Quaternion.y + y * Quaternion.w + z * Quaternion.x - x * Quaternion.z,
                       w * Quaternion.z + z * Quaternion.w + x * Quaternion.y - y * Quaternion.x,
                       w * Quaternion.w - x * Quaternion.x - y * Quaternion.y - z * Quaternion.z);
}

inline CQuaternion CQuaternion::operator *(float scale) const
{
	return CQuaternion(x*scale, y*scale, z*scale, w*scale); 
}

inline CQuaternion CQuaternion::operator +(const CQuaternion& Quaternion) const
{
	return CQuaternion(x + Quaternion.x, y + Quaternion.y, z + Quaternion.z, w + Quaternion.w); 
}

/////////////////////////////////////////////////////////////
/// operateur de multiplication - affectation
///
/// \param Quaternion : Quaternion a multiplier
///
/// \return Resultat de l'operation
///
////////////////////////////////////////////////////////////
inline const CQuaternion& CQuaternion::operator *=(const CQuaternion& Quaternion)
{
    *this = *this * Quaternion;

    return *this;
}


float CQuaternion::Dot(const CQuaternion& q1, const CQuaternion& q2)
{
	return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

//============ Interpolation
CQuaternion CQuaternion::lerp(const CQuaternion &q1, const CQuaternion &q2, float t)
{
	 CQuaternion res = (q1*(1-t) + q2*t);
	 res.Normalize();
	 return res;
}

CQuaternion CQuaternion::slerp(const CQuaternion &q1, const CQuaternion &q2, float t)
{
	CQuaternion q3;
	float dot = CQuaternion::Dot(q1, q2);

	/*	dot = cos(theta)
		if (dot < 0), q1 and q2 are more than 90 degrees apart,
		so we can invert one to reduce spinning	*/
	if (dot < 0)
	{
		dot = -dot;
		q3 = q2*-1;
	} else q3 = q2;
	
	if (dot < 0.95f)
	{
		float angle = acosf(dot);
		return (q1*sinf(angle*(1-t)) + q3*sinf(angle*t))*(1.0/sinf(angle));
	} else // if the angle is small, use linear interpolation								
		return lerp(q1,q3,t);	
}

CQuaternion CQuaternion::slerpNoInvert(const CQuaternion &q1, const CQuaternion &q2, float t)
{
	float dot = CQuaternion::Dot(q1, q2);

	if (dot > -0.95f && dot < 0.95f)
	{
		float angle = acosf(dot);			
		return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))*(1.0/sinf(angle));
	} else  // if the angle is small, use linear interpolation								
		return lerp(q1,q2,t);
}

CQuaternion CQuaternion::squad(const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &a,const CQuaternion &b,float t)
{
	CQuaternion c= slerpNoInvert(q1,q2,t),
		       d= slerpNoInvert(a,b,t);		
	return slerpNoInvert(c,d,2*t*(1-t));
}

/////////////////////////////////////////////////////////////
/// Surcharge de l'op�rateur >> entre un flux et un quaternion
///
/// \param  Stream :     Flux d'entr�e
/// \param Quaternion : Quaternion
///
/// \return R�f�rence sur le flux d'entr�e
///
////////////////////////////////////////////////////////////
std::istream& operator >>(std::istream& Stream, CQuaternion& Quaternion)
{
    return Stream >> Quaternion.x >> Quaternion.y >> Quaternion.z >> Quaternion.w;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op�rateur << entre un flux et un quaternion
///
/// \param Stream :     Flux de sortie
/// \param Quaternion : Quaternion
///
/// \return R�f�rence sur le flux de sortie
///
////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& Stream, const CQuaternion& Quaternion)
{
    return Stream << Quaternion.x << " " << Quaternion.y << " " << Quaternion.z << " " << Quaternion.w;
}
