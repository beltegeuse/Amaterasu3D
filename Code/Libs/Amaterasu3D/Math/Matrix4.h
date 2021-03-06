#ifndef MATRIX4_H
#define MATRIX4_H

//==========================================================
// En-tetes
//==========================================================
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Vector2.h>
#include <Math/Matrix3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
namespace ama3D
{
namespace Math
{
////////////////////////////////////////////////////////////
/// Classe des matrices � 4 composantes r�elles
////////////////////////////////////////////////////////////
class CMatrix4
{
public:

	//----------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------
	CMatrix4(float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m14 =
			0.0f, float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f,
			float m24 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 =
					1.0f, float m34 = 0.0f, float m41 = 0.0f, float m42 = 0.0f,
			float m43 = 0.0f, float m44 = 1.0f);

	void LoadFromOpenGL(int matrixType);

	//----------------------------------------------------------
	// Met la matrice � l'identit�
	//----------------------------------------------------------
	void Identity();

	//----------------------------------------------------------
	// Calcule le d�terminant de la matrice
	//----------------------------------------------------------
	float Determinant() const;

	//----------------------------------------------------------
	// Transpose la matrice
	//----------------------------------------------------------
	CMatrix4 Transpose() const;

	//----------------------------------------------------------
	// Inverse la matrice
	//----------------------------------------------------------
	CMatrix4 Inverse() const;

	//----------------------------------------------------------
	// Construit une matrice de translation
	//----------------------------------------------------------
	void SetTranslation(float x, float y, float z);

	//----------------------------------------------------------
	// Construit une matrice de mise � l'�chelle
	//----------------------------------------------------------
	void SetScaling(float x, float y, float z);

	//----------------------------------------------------------
	// Construit une matrice de rotation centr�e en (0, 0, 0)
	//----------------------------------------------------------
	void SetRotationX(float Angle);
	void SetRotationY(float Angle);
	void SetRotationZ(float Angle);

	//----------------------------------------------------------
	// Construit une matrice de rotation en sp�cifiant le centre
	//----------------------------------------------------------
	void SetRotationX(float Angle, const TVector3F& Center);
	void SetRotationY(float Angle, const TVector3F& Center);
	void SetRotationZ(float Angle, const TVector3F& Center);

	//----------------------------------------------------------
	// Renvoie la translation contenue dans la matrice
	//----------------------------------------------------------
	TVector3F GetTranslation() const;
	TVector3F GetScale() const;

	//----------------------------------------------------------
	// Transforme un vecteur
	//----------------------------------------------------------
	TVector3F Transform(const TVector3F& v, float w = 1.0f) const;
	TVector4F Transform(const TVector4F& v) const;

	//----------------------------------------------------------
	// Construit une matrice ortho non-centr�e
	//----------------------------------------------------------
	void OrthoOffCenter(float Left, float Top, float Right, float Bottom);

	//----------------------------------------------------------
	// Construit une matrice de projection perspective
	//----------------------------------------------------------
	static CMatrix4 CreatePerspectiveFOV(float Fov, float Ratio, float Near,
			float Far);

	//----------------------------------------------------------
	// Build an projection matrix
	//----------------------------------------------------------
	static CMatrix4 CreatePerspectiveProjection(const Math::TVector2F& LB,
			const Math::TVector2F& RT, float Near, float Far);

	//----------------------------------------------------------
	// Construit une matrice de vue
	//----------------------------------------------------------
	void LookAt(const TVector3F& From, const TVector3F& To,
			const TVector3F& Up = TVector3F(0, 1, 0));

	//----------------------------------------------------------
	// Extract a 3x3 matrix
	//----------------------------------------------------------
	Math::CMatrix3 ExtractSubMatrix() const;

	//----------------------------------------------------------
	// Op�rateurs + et - unaires
	//----------------------------------------------------------
	CMatrix4 operator +() const;
	CMatrix4 operator -() const;

	//----------------------------------------------------------
	// Op�rateurs + et - binaires
	//----------------------------------------------------------
	CMatrix4 operator +(const CMatrix4& m) const;
	CMatrix4 operator -(const CMatrix4& m) const;

	//----------------------------------------------------------
	// Op�rateurs += et -=
	//----------------------------------------------------------
	const CMatrix4& operator +=(const CMatrix4& m);
	const CMatrix4& operator -=(const CMatrix4& m);

	//----------------------------------------------------------
	// Op�rateur * et *=
	//----------------------------------------------------------
	CMatrix4 operator *(const CMatrix4& m) const;
	const CMatrix4& operator *=(const CMatrix4& m);

	//----------------------------------------------------------
	// Op�rateurs *= et /= avec un scalaire
	//----------------------------------------------------------
	const CMatrix4& operator *=(float t);
	const CMatrix4& operator /=(float t);

	//----------------------------------------------------------
	// Op�rateurs de comparaison
	//----------------------------------------------------------
	bool operator ==(const CMatrix4& m) const;
	bool operator !=(const CMatrix4& m) const;

	//----------------------------------------------------------
	// Op�rateurs d'acc�s index� aux �l�ments
	//----------------------------------------------------------
	float& operator ()(std::size_t i, std::size_t j);
	const float& operator ()(std::size_t i, std::size_t j) const;

	//----------------------------------------------------------
	// Op�rateur de cast en float*
	//----------------------------------------------------------
	operator float*();
	operator const float*() const;

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	float a11, a21, a31, a41; ///< 1ere ligne de la matrice
	float a12, a22, a32, a42; ///< 2eme ligne de la matrice
	float a13, a23, a33, a43; ///< 3eme ligne de la matrice
	float a14, a24, a34, a44; ///< 4eme ligne de la matrice
};

//==========================================================
// Fonctions globales relatives aux matrices
//==========================================================
CMatrix4 operator *(const CMatrix4& m, float t);
CMatrix4 operator *(float t, const CMatrix4& m);
CMatrix4 operator /(const CMatrix4& m, float t);
std::istream& operator >>(std::istream& Stream, CMatrix4& Mat);
std::ostream& operator <<(std::ostream& Stream, const CMatrix4& Mat);

#include "Matrix4.inl"

} // namespace Math
}
#endif // MATRIX4_H
