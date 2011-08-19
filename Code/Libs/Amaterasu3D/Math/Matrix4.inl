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


#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

/////////////////////////////////////////////////////////////
/// Constructeur par d���faut
///
/// \param m11 - m44 : Composantes de la matrice
///
////////////////////////////////////////////////////////////
inline CMatrix4::CMatrix4(float m11, float m12, float m13, float m14,
                          float m21, float m22, float m23, float m24,
                          float m31, float m32, float m33, float m34,
                          float m41, float m42, float m43, float m44) :
a11(m11), a21(m21), a31(m31), a41(m41),
a12(m12), a22(m22), a32(m32), a42(m42),
a13(m13), a23(m23), a33(m33), a43(m43),
a14(m14), a24(m24), a34(m34), a44(m44)
{

}

inline void CMatrix4::LoadFromOpenGL(int matrixType)
{
	float mat[16];
	glGetFloatv(matrixType, mat);
	a11 = mat[0]; a12 = mat[1]; a13 = mat[2]; a14 = mat[3];
	a21 = mat[4]; a22 = mat[5]; a23 = mat[6]; a24 = mat[7];
	a31 = mat[8]; a32 = mat[9]; a33 = mat[10]; a34 = mat[11];
	a41 = mat[12]; a42 = mat[13]; a43 = mat[14]; a44 = mat[15];
}

/////////////////////////////////////////////////////////////
/// Met la matrice ��� l'identit���
///
////////////////////////////////////////////////////////////
inline void CMatrix4::Identity()
{
    a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
    a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f;
    a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Calcule le d���terminant de la matrice
///
/// \return Valeur du d���terminant
///
////////////////////////////////////////////////////////////
inline float CMatrix4::Determinant() const
{
    float A = a22 * (a33 * a44 - a43 * a34) - a32 * (a23 * a44 - a43 * a24) + a42 * (a23 * a34 - a33 * a24);
    float B = a12 * (a33 * a44 - a43 * a34) - a32 * (a13 * a44 - a43 * a14) + a42 * (a13 * a34 - a33 * a14);
    float C = a12 * (a23 * a44 - a43 * a24) - a22 * (a13 * a44 - a43 * a14) + a42 * (a13 * a24 - a23 * a14);
    float D = a12 * (a23 * a34 - a33 * a24) - a22 * (a13 * a34 - a33 * a14) + a32 * (a13 * a24 - a23 * a14);

    return a11 * A - a21 * B + a31 * C - a41 * D;
}


/////////////////////////////////////////////////////////////
/// Transpose la matrice
///
/// \return Matrice transpos���e
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::Transpose() const
{
    return CMatrix4(a11, a21, a31, a41,
                    a12, a22, a32, a42,
                    a13, a23, a33, a43,
                    a14, a24, a34, a44);
}


/////////////////////////////////////////////////////////////
/// Inverse la matrice si son d���terminant est non nul
///
/// \return Matrice inverse
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::Inverse() const
{
    CMatrix4 Ret;
    float Det = Determinant();

    if (std::fabs(Det) > std::numeric_limits<float>::epsilon())
    {
        Ret.a11 =  (a22 * (a33 * a44 - a34 * a43) - a32 * (a23 * a44 - a43 * a24) + a42 * (a23 * a34 - a33 *  a24)) / Det;
        Ret.a12 = -(a12 * (a33 * a44 - a43 * a34) - a32 * (a13 * a44 - a43 * a14) + a42 * (a13 * a34 - a33 *  a14)) / Det;
        Ret.a13 =  (a12 * (a23 * a44 - a43 * a24) - a22 * (a13 * a44 - a43 * a14) + a42 * (a13 * a24 - a23 *  a14)) / Det;
        Ret.a14 = -(a12 * (a23 * a34 - a33 * a24) - a22 * (a13 * a34 - a33 * a14) + a32 * (a13 * a24 - a23 *  a14)) / Det;

        Ret.a21 = -(a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 *  a41)) / Det;
        Ret.a22 =  (a11 * (a33 * a44 - a34 * a43) - a13 * (a31 * a44 - a34 * a41) + a14 * (a31 * a43 - a33 *  a41)) / Det;
        Ret.a23 = -(a11 * (a23 * a44 - a24 * a43) - a13 * (a21 * a44 - a24 * a41) + a14 * (a21 * a43 - a23 *  a41)) / Det;
        Ret.a24 =  (a11 * (a23 * a34 - a24 * a33) - a13 * (a21 * a34 - a24 * a31) + a14 * (a21 * a33 - a23 *  a31)) / Det;

        Ret.a31 =  (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 *  a41)) / Det;
        Ret.a32 = -(a11 * (a32 * a44 - a34 * a42) - a12 * (a31 * a44 - a34 * a41) + a14 * (a31 * a42 - a32 *  a41)) / Det;
        Ret.a33 =  (a11 * (a22 * a44 - a24 * a42) - a12 * (a21 * a44 - a24 * a41) + a14 * (a21 * a42 - a22 *  a41)) / Det;
        Ret.a34 = -(a11 * (a22 * a34 - a24 * a32) - a12 * (a21 * a34 - a24 * a31) + a14 * (a21 * a32 - a22 *  a31)) / Det;

        Ret.a41 = -(a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 *  a41)) / Det;
        Ret.a42 =  (a11 * (a32 * a43 - a33 * a42) - a12 * (a31 * a43 - a33 * a41) + a13 * (a31 * a42 - a32 *  a41)) / Det;
        Ret.a43 = -(a11 * (a22 * a43 - a23 * a42) - a12 * (a21 * a43 - a23 * a41) + a13 * (a21 * a42 - a22 *  a41)) / Det;
        Ret.a44 =  (a11 * (a22 * a33 - a23 * a32) - a12 * (a21 * a33 - a23 * a31) + a13 * (a21 * a32 - a22 *  a31)) / Det;
    }

    return Ret;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de translation
///
/// \param x : Translation sur X
/// \param y : Translation sur Y
/// \param z : Translation sur Z
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetTranslation(float x, float y, float z)
{
    a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = x;
    a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = y;
    a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = z;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de mise ��� l'���chelle
///
/// \param x : Facteur de redimensionnement sur l'axe X
/// \param y : Facteur de redimensionnement sur l'axe Y
/// \param z : Facteur de redimensionnement sur l'axe Z
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetScaling(float x, float y, float z)
{
    a11 = x;    a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
    a21 = 0.0f; a22 = y;    a23 = 0.0f; a24 = 0.0f;
    a31 = 0.0f; a32 = 0.0f; a33 = z;    a34 = 0.0f;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe X
///
/// \param Angle : Angle de rotation en radians
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationX(float Angle)
{
    float Cos = std::cos(Angle);
    float Sin = std::sin(Angle);

    a11 = 1.0f; a12 = 0.0f; a13 = 0.0f; a14 = 0.0f;
    a21 = 0.0f; a22 = Cos;  a23 = Sin;  a24 = 0.0f;
    a31 = 0.0f; a32 = -Sin; a33 = Cos;  a34 = 0.0f;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe Y
///
/// \param Angle : Angle de rotation en radians
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationY(float Angle)
{
    float Cos = std::cos(Angle);
    float Sin = std::sin(Angle);

    a11 = Cos;  a12 = 0.0f; a13 = -Sin; a14 = 0.0f;
    a21 = 0.0f; a22 = 1.0f; a23 = 0.0f; a24 = 0.0f;
    a31 = Sin;  a32 = 0.0f; a33 = Cos;  a34 = 0.0f;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe Z
///
/// \param Angle : Angle de rotation en radians
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationZ(float Angle)
{
    float Cos = std::cos(Angle);
    float Sin = std::sin(Angle);

    a11 = Cos;  a12 = Sin;  a13 = 0.0f; a14 = 0.0f;
    a21 = -Sin; a22 = Cos;  a23 = 0.0f; a24 = 0.0f;
    a31 = 0.0f; a32 = 0.0f; a33 = 1.0f; a34 = 0.0f;
    a41 = 0.0f; a42 = 0.0f; a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe X
///
/// \param Angle :  Angle de rotation en radians
/// \param Center : Centre de rotation
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationX(float Angle, const TVector3F& Center)
{
    CMatrix4 Tr1, Tr2, Rot;

    Tr1.SetTranslation(Center.x, Center.y, Center.z);
    Tr2.SetTranslation(-Center.x, -Center.y, -Center.z);
    Rot.SetRotationX(Angle);

    *this = Tr1 * Rot * Tr2;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe Y
///
/// \param Angle :  Angle de rotation en radians
/// \param Center : Centre de rotation
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationY(float Angle, const TVector3F& Center)
{
    CMatrix4 Tr1, Tr2, Rot;

    Tr1.SetTranslation(Center.x, Center.y, Center.z);
    Tr2.SetTranslation(-Center.x, -Center.y, -Center.z);
    Rot.SetRotationY(Angle);

    *this = Tr1 * Rot * Tr2;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de rotation sur l'axe Z
///
/// \param Angle :  Angle de rotation en radians
/// \param Center : Centre de rotation
///
////////////////////////////////////////////////////////////
inline void CMatrix4::SetRotationZ(float Angle, const TVector3F& Center)
{
    CMatrix4 Tr1, Tr2, Rot;

    Tr1.SetTranslation(Center.x, Center.y, Center.z);
    Tr2.SetTranslation(-Center.x, -Center.y, -Center.z);
    Rot.SetRotationZ(Angle);

    *this = Tr1 * Rot * Tr2;
}


/////////////////////////////////////////////////////////////
/// Renvoie la translation contenue dans la matrice
///
/// \return Vecteur translation
///
////////////////////////////////////////////////////////////
inline TVector3F CMatrix4::GetTranslation() const
{
    return TVector3F(a14, a24, a34);
}

inline TVector3F CMatrix4::GetScale() const
{
	return TVector3F(glm::length(TVector3F(a11,a12,a13)), glm::length(TVector3F(a21,a22,a23)), glm::length(TVector3F(a31,a32,a33)));
}


/////////////////////////////////////////////////////////////
/// Transforme un vecteur ��� 3 composantes
///
/// \param v : Vecteur ��� transformer
/// \param w : Composante w
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline TVector3F CMatrix4::Transform(const TVector3F& v, float w) const
{
    return TVector3F(v.x * a11 + v.y * a21 + v.z * a31 + w * a41,
                     v.x * a12 + v.y * a22 + v.z * a32 + w * a42,
                     v.x * a13 + v.y * a23 + v.z * a33 + w * a43);
}


/////////////////////////////////////////////////////////////
/// Transforme un vecteur ��� 4 composantes
///
/// \param v : Vecteur ��� transformer
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline TVector4F CMatrix4::Transform(const TVector4F& v) const
{
    return TVector4F(v.x * a11 + v.y * a21 + v.z * a31 + v.w * a41,
                     v.x * a12 + v.y * a22 + v.z * a32 + v.w * a42,
                     v.x * a13 + v.y * a23 + v.z * a33 + v.w * a43,
                     v.x * a14 + v.y * a24 + v.z * a34 + v.w * a44);
}


/////////////////////////////////////////////////////////////
/// Construit une matrice ortho non centr���e
///
/// \param Left :   Gauche
/// \param Top :    Haut
/// \param Right :  Droite
/// \param Bottom : Bas
///
////////////////////////////////////////////////////////////
inline void CMatrix4::OrthoOffCenter(float Left, float Top, float Right, float Bottom)
{
    a11 = 2 / (Right - Left); a12 = 0.0f;               a13 = 0.0f; a14 = (Left + Right) / (Left - Right);
    a21 = 0.0f;               a22 = 2 / (Top - Bottom); a23 = 0.0f; a24 = (Bottom + Top) / (Bottom - Top);
    a31 = 0.0f;               a32 = 0.0f;               a33 = 1.0f; a34 = 0.0f;
    a41 = 0.0f;               a42 = 0.0f;               a43 = 0.0f; a44 = 1.0f;
}


/////////////////////////////////////////////////////////////
/// Construit une matrice de projection perspective
///
/// \param Fov :   Champ de vision en degree
/// \param Ratio : Ratio largeur / hauteur
/// \param Near :  Valeur du plan rapproch���
/// \param Far :   Valeur du plan ���loign���
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::CreatePerspectiveFOV(float Fov, float Ratio, float Near, float Far)
{
	CMatrix4 res;

	float f = 1.0 / tan((Fov / 2) * M_PI / 180);
	res.a11 = f / Ratio; res.a12 = 0.0f;   res.a13 = 0.0f;  res.a14 = 0.0f;
	res.a21 = 0.0f;   res.a22 = f; res.a23 = 0.0f;  res.a24 = 0.0f;
	res.a31 = 0.0f;   res.a32 = 0.0f;   res.a33 = (Near + Far) / (Near - Far); res.a34 = (2 * Near * Far) / (Near - Far);
    res.a41 = 0.0f;   res.a42 = 0.0f;   res.a43 = -1.0f;  res.a44 = 0.0f;
    
    return res;
}

inline CMatrix4 CMatrix4::CreatePerspectiveProjection(const Math::TVector2F& LB, const Math::TVector2F& RT,float Near, float Far)
{
	CMatrix4 res;

	float Width = LB.x - RT.x;
	float Height = LB.y - RT.y;
	float Dynamic = Far - Near;

	res.a11 = 2*Near/Width; res.a12 = 0.0f; res.a13 = (LB.x+RT.x)/Width; res.a14 = 0.0f;
	res.a21 = 0.0f; res.a22 = 2*Near/Height; res.a23 = (LB.y+RT.y)/Height; res.a24 = 0.0f;
	res.a31 = 0.0f; res.a32 = 0.0f; res.a33 = -(Near+Far)/Dynamic; res.a34 = -2*Far*Near/Dynamic;
	res.a41 = 0.0f; res.a42 = 0.0f; res.a43 = -1.0f; res.a44 = 0.0f;

	return res;
}

/////////////////////////////////////////////////////////////
/// Construit une matrice de vue
///
/// \param From : Position de la cam���ra
/// \param To :   Cible de la cam���ra
/// \param Up :   Vecteur up ((0, 1, 0) par d���faut)
///
////////////////////////////////////////////////////////////
inline void CMatrix4::LookAt(const TVector3F& From, const TVector3F& To, const TVector3F& Up)
{
    // TODO : g���rer le cas o��� (To - From) et Up sont colin���aires

    TVector3F ZAxis = To - From;
    ZAxis = glm::normalize(ZAxis);
    TVector3F XAxis = glm::cross(Up,ZAxis);
    XAxis = glm::normalize(XAxis);
    TVector3F YAxis = glm::cross(ZAxis,XAxis);

    a11 = -XAxis.x; a12 = -XAxis.y; a13 = -XAxis.z; a14 = glm::dot(XAxis,From);
    a21 = YAxis.x; a22 = YAxis.y; a23 = YAxis.z; a24 = -glm::dot(YAxis,From);
    a31 = -ZAxis.x; a32 = -ZAxis.y; a33 = -ZAxis.z; a34 = glm::dot(ZAxis,From);
    a41 = 0.0f;    a42 = 0.0f;    a43 = 0.0f;    a44 = 1.0f;
}
////////////////////////////////////////////////////////////
/// Extract subMatrix 3x3
////////////////////////////////////////////////////////////
inline Math::CMatrix3 CMatrix4::ExtractSubMatrix() const
{
	Math::CMatrix3 m(a11, a12, a13,
				    a21, a22, a23,
				    a13, a23, a33);
	return m;
}


/////////////////////////////////////////////////////////////
/// Op���rateur + unaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::operator +() const
{
    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur - unaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::operator -() const
{
    return CMatrix4(-a11, -a12, -a13, -a14,
                    -a21, -a22, -a23, -a24,
                    -a31, -a32, -a33, -a34,
                    -a41, -a42, -a43, -a44);
}


/////////////////////////////////////////////////////////////
/// Op���rateur + binaire
///
/// \param m : Matrice ��� additionner
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::operator +(const CMatrix4& m) const
{
    return CMatrix4(a11 + m.a11, a12 + m.a12, a13 + m.a13, a14 + m.a14,
                    a21 + m.a21, a22 + m.a22, a23 + m.a23, a24 + m.a24,
                    a31 + m.a31, a32 + m.a32, a33 + m.a33, a34 + m.a34,
                    a41 + m.a41, a42 + m.a42, a43 + m.a43, a44 + m.a44);
}


/////////////////////////////////////////////////////////////
/// Op���rateur - binaire
///
/// \param m : Matrice ��� soustraire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::operator -(const CMatrix4& m) const
{
    return CMatrix4(a11 - m.a11, a12 - m.a12, a13 - m.a13, a14 - m.a14,
                    a21 - m.a21, a22 - m.a22, a23 - m.a23, a24 - m.a24,
                    a31 - m.a31, a32 - m.a32, a33 - m.a33, a34 - m.a34,
                    a41 - m.a41, a42 - m.a42, a43 - m.a43, a44 - m.a44);
}


/////////////////////////////////////////////////////////////
/// Op���rateur +=
///
/// \param m : Matrice ��� additionner
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline const CMatrix4& CMatrix4::operator +=(const CMatrix4& m)
{
    a11 += m.a11; a12 += m.a12; a13 += m.a13; a14 += m.a14;
    a21 += m.a21; a22 += m.a22; a23 += m.a23; a24 += m.a24;
    a31 += m.a31; a32 += m.a32; a33 += m.a33; a34 += m.a34;
    a41 += m.a41; a42 += m.a42; a43 += m.a43; a44 += m.a44;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur -=
///
/// \param m : Matrice ��� soustraire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline const CMatrix4& CMatrix4::operator -=(const CMatrix4& m)
{
    a11 -= m.a11; a12 -= m.a12; a13 -= m.a13; a14 -= m.a14;
    a21 -= m.a21; a22 -= m.a22; a23 -= m.a23; a24 -= m.a24;
    a31 -= m.a31; a32 -= m.a32; a33 -= m.a33; a34 -= m.a34;
    a41 -= m.a41; a42 -= m.a42; a43 -= m.a43; a44 -= m.a44;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur *
///
/// \param m : Matrice ��� multiplier
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 CMatrix4::operator *(const CMatrix4& m) const
{
    return CMatrix4(a11 * m.a11 + a21 * m.a12 + a31 * m.a13 + a41 * m.a14,
                    a12 * m.a11 + a22 * m.a12 + a32 * m.a13 + a42 * m.a14,
                    a13 * m.a11 + a23 * m.a12 + a33 * m.a13 + a43 * m.a14,
                    a14 * m.a11 + a24 * m.a12 + a34 * m.a13 + a44 * m.a14,

                    a11 * m.a21 + a21 * m.a22 + a31 * m.a23 + a41 * m.a24,
                    a12 * m.a21 + a22 * m.a22 + a32 * m.a23 + a42 * m.a24,
                    a13 * m.a21 + a23 * m.a22 + a33 * m.a23 + a43 * m.a24,
                    a14 * m.a21 + a24 * m.a22 + a34 * m.a23 + a44 * m.a24,

                    a11 * m.a31 + a21 * m.a32 + a31 * m.a33 + a41 * m.a34,
                    a12 * m.a31 + a22 * m.a32 + a32 * m.a33 + a42 * m.a34,
                    a13 * m.a31 + a23 * m.a32 + a33 * m.a33 + a43 * m.a34,
                    a14 * m.a31 + a24 * m.a32 + a34 * m.a33 + a44 * m.a34,

                    a11 * m.a41 + a21 * m.a42 + a31 * m.a43 + a41 * m.a44,
                    a12 * m.a41 + a22 * m.a42 + a32 * m.a43 + a42 * m.a44,
                    a13 * m.a41 + a23 * m.a42 + a33 * m.a43 + a43 * m.a44,
                    a14 * m.a41 + a24 * m.a42 + a34 * m.a43 + a44 * m.a44);
}


/////////////////////////////////////////////////////////////
/// Op���rateur *=
///
/// \param m : Matrice ��� multiplier
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline const CMatrix4& CMatrix4::operator *=(const CMatrix4& m)
{
    *this = *this * m;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur *= avec un scalaire
///
/// \param t : Scalaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline const CMatrix4& CMatrix4::operator *=(float t)
{
    a11 *= t; a12 *= t; a13 *= t; a14 *= t;
    a21 *= t; a22 *= t; a23 *= t; a24 *= t;
    a31 *= t; a32 *= t; a33 *= t; a34 *= t;
    a41 *= t; a42 *= t; a43 *= t; a44 *= t;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur /= avec un scalaire
///
/// \param t : Scalaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline const CMatrix4& CMatrix4::operator /=(float t)
{
    a11 /= t; a12 /= t; a13 /= t; a14 /= t;
    a21 /= t; a22 /= t; a23 /= t; a24 /= t;
    a31 /= t; a32 /= t; a33 /= t; a34 /= t;
    a41 /= t; a42 /= t; a43 /= t; a44 /= t;

    return *this;
}


/////////////////////////////////////////////////////////////
/// Op���rateur de comparaison ==
///
/// \param m : Matrice ��� comparer
///
/// \return True si les deux matrices sont identiques
///
////////////////////////////////////////////////////////////
inline bool CMatrix4::operator ==(const CMatrix4& m) const
{
    return ((std::fabs(a11 - m.a11) < std::numeric_limits<float>::epsilon()) && (std::fabs(a12 - m.a12) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a13 - m.a13) < std::numeric_limits<float>::epsilon()) && (std::fabs(a14 - m.a14) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a21 - m.a21) < std::numeric_limits<float>::epsilon()) && (std::fabs(a22 - m.a22) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a23 - m.a23) < std::numeric_limits<float>::epsilon()) && (std::fabs(a24 - m.a24) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a31 - m.a31) < std::numeric_limits<float>::epsilon()) && (std::fabs(a32 - m.a32) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a33 - m.a33) < std::numeric_limits<float>::epsilon()) && (std::fabs(a34 - m.a34) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a41 - m.a41) < std::numeric_limits<float>::epsilon()) && (std::fabs(a42 - m.a42) < std::numeric_limits<float>::epsilon()) &&
            (std::fabs(a43 - m.a43) < std::numeric_limits<float>::epsilon()) && (std::fabs(a44 - m.a44) < std::numeric_limits<float>::epsilon()));
}


/////////////////////////////////////////////////////////////
/// Op���rateur de comparaison !=
///
/// \param m : Matrice ��� comparer
///
/// \return True si les deux matrices sont diff���rentes
///
////////////////////////////////////////////////////////////
inline bool CMatrix4::operator !=(const CMatrix4& m) const
{
    return !(*this == m);
}


/////////////////////////////////////////////////////////////
/// Op���rateur d'acc���s index��� aux ���l���ments
///
/// \param i : Ligne de la composante ��� r���cup���rer
/// \param j : Colonne de la composante ��� r���cup���rer
///
/// \return R���f���rence sur la composante (i, j) de la matrice
///
////////////////////////////////////////////////////////////
inline float& CMatrix4::operator ()(std::size_t i, std::size_t j)
{
    return operator float*()[i + 4 * j];
}


/////////////////////////////////////////////////////////////
/// Operateur d'acces indexe aux elements - constant
///
/// \param i : Ligne de la composante a recuperer
/// \param j : Colonne de la composante a recuperer
///
/// \return Reference sur la composante (i, j) de la matrice
///
////////////////////////////////////////////////////////////
inline const float& CMatrix4::operator ()(std::size_t i, std::size_t j) const
{
    return operator const float*()[i + 4 * j];
}


/////////////////////////////////////////////////////////////
/// Op���rateur de cast en const float*
///
/// \return Pointeur constant sur les composantes du vecteur
///
////////////////////////////////////////////////////////////
inline CMatrix4::operator const float*() const
{
    return &a11;
}


/////////////////////////////////////////////////////////////
/// Op���rateur de cast en float*
///
/// \return Pointeur sur les composantes du vecteur
///
////////////////////////////////////////////////////////////
inline CMatrix4::operator float*()
{
    return &a11;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op���rateur * entre une matrice et un scalaire
///
/// \param m : Matrice
/// \param t : Scalaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 operator *(const CMatrix4& m, float t)
{
    return CMatrix4(m.a11 * t, m.a12 * t, m.a13 * t, m.a14 * t,
                    m.a21 * t, m.a22 * t, m.a23 * t, m.a24 * t,
                    m.a31 * t, m.a32 * t, m.a33 * t, m.a34 * t,
                    m.a41 * t, m.a42 * t, m.a43 * t, m.a44 * t);
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op���rateur * entre un scalaire et une matrice
///
/// \param t : Scalaire
/// \param m : Matrice
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 operator *(float t, const CMatrix4& m)
{
    return m * t;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op���rateur / entre une matrice et un scalaire
///
/// \param m : Matrice
/// \param t : Scalaire
///
/// \return R���sultat de l'op���ration
///
////////////////////////////////////////////////////////////
inline CMatrix4 operator /(const CMatrix4& m, float t)
{
    return CMatrix4(m.a11 / t, m.a12 / t, m.a13 / t, m.a14 / t,
                    m.a21 / t, m.a22 / t, m.a23 / t, m.a24 / t,
                    m.a31 / t, m.a32 / t, m.a33 / t, m.a34 / t,
                    m.a41 / t, m.a42 / t, m.a43 / t, m.a44 / t);
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op���rateur >> entre un flux et une matrice
///
/// \param  Stream : Flux d'entr���e
/// \param Mat :    Matrice
///
/// \return R���f���rence sur le flux d'entr���e
///
////////////////////////////////////////////////////////////
inline std::istream& operator >>(std::istream& Stream, CMatrix4& Mat)
{
    Stream >> Mat.a11 >> Mat.a12 >> Mat.a13 >> Mat.a14;
    Stream >> Mat.a21 >> Mat.a22 >> Mat.a23 >> Mat.a24;
    Stream >> Mat.a31 >> Mat.a32 >> Mat.a33 >> Mat.a34;
    Stream >> Mat.a41 >> Mat.a42 >> Mat.a43 >> Mat.a44;

    return Stream;
}


/////////////////////////////////////////////////////////////
/// Surcharge de l'op���rateur << entre un flux et une matrice
///
/// \param Stream : Flux de sortie
/// \param Mat :    Matrice
///
/// \return R���f���rence sur le flux de sortie
///
////////////////////////////////////////////////////////////
inline std::ostream& operator <<(std::ostream& Stream, const CMatrix4& Mat)
{
    Stream << Mat.a11 << " " << Mat.a12 << " " << Mat.a13 << " " << Mat.a14 << std::endl;
    Stream << Mat.a21 << " " << Mat.a22 << " " << Mat.a23 << " " << Mat.a24 << std::endl;
    Stream << Mat.a31 << " " << Mat.a32 << " " << Mat.a33 << " " << Mat.a34 << std::endl;
    Stream << Mat.a41 << " " << Mat.a42 << " " << Mat.a43 << " " << Mat.a44 << std::endl;

    return Stream;
}
