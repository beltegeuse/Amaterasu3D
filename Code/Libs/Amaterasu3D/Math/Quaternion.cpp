
#include <Math/Quaternion.h>

namespace Math {
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

}
