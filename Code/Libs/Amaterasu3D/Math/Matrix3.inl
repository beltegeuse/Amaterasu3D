inline CMatrix3::CMatrix3(float m11, float m12, float m13,
                          float m21, float m22, float m23,
                          float m31, float m32, float m33) :
a11(m11), a12(m12), a13(m13),
a21(m21), a22(m22), a23(m23),
a31(m31), a32(m32), a33(m33)
{
}

inline CMatrix3::operator float*()
{
    return &a11;
}

inline CMatrix3::operator const float*() const
{
    return &a11;
}

inline std::istream& operator >>(std::istream& Stream, CMatrix3& Mat)
{
	Stream >> Mat.a11 >> Mat.a12 >> Mat.a13;
	Stream >> Mat.a21 >> Mat.a22 >> Mat.a23;
	Stream >> Mat.a31 >> Mat.a32 >> Mat.a33;

	return Stream;
}

inline std::ostream& operator <<(std::ostream& Stream, const CMatrix3& Mat)
{
	Stream << Mat.a11 << " " << Mat.a12 << " " << Mat.a13 << std::endl;
	Stream << Mat.a21 << " " << Mat.a22 << " " << Mat.a23 << std::endl;
	Stream << Mat.a31 << " " << Mat.a32 << " " << Mat.a33 << std::endl;

	return Stream;
}