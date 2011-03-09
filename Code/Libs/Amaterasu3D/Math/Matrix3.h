#pragma once


namespace Math
{
	class CMatrix3
	{
	public:
		 CMatrix3(float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f,
				  float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f,
				  float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f);

		 operator float*();
		 operator const float*() const;

		 float a11, a12, a13; ///< 1ere ligne de la matrice
		 float a21, a22, a23; ///< 2eme ligne de la matrice
		 float a31, a32, a33; ///< 3eme ligne de la matrice
	};

    #include "Matrix3.inl"
}
