#include <boost/test/unit_test.hpp>
#include <Math/Vector3.h>
#include <test_helper.h>

BOOST_AUTO_TEST_CASE(vec3_length)
{
	ama3D::Math::TVector3F vec(1.0,1.0,1.0);
	BOOST_CHECK_FLOAT(sqrt(3.0), vec.Length());
	BOOST_CHECK_EQUAL(3.0, vec.LengthSq());

	ama3D::Math::TVector3F vec2(2.0,1.0,0.0);
	BOOST_CHECK_FLOAT(sqrt(5.0), vec2.Length());
	BOOST_CHECK_EQUAL(5.0, vec2.LengthSq());
}
BOOST_AUTO_TEST_CASE(vec3_add)
{
	ama3D::Math::TVector3F vec(1.0,1.0,0.0);
	ama3D::Math::TVector3F resWait(2.0,2.0,0.0);
	ama3D::Math::TVector3F res = vec + vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(vec3_sub)
{
	ama3D::Math::TVector3F vec(1.0,1.0,-1.0);
	ama3D::Math::TVector3F resWait(0.0,0.0,0.0);
	ama3D::Math::TVector3F res = vec - vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(vec3_normalize)
{
	ama3D::Math::TVector3F vec(10.0,5.0,1.0);
	vec.Normalize();
	BOOST_CHECK_EQUAL(vec.Length(), 1.0);
}
BOOST_AUTO_TEST_CASE(vec3_dot)
{
	ama3D::Math::TVector3F vec1_1(1.0,0.0,0.0);
	ama3D::Math::TVector3F vec1_2(0.0,1.0,0.0);
	BOOST_CHECK_EQUAL(vec1_1*vec1_2,0.0);

	ama3D::Math::TVector3F vec2_1(1.0,1.0,0.0);
	vec2_1.Normalize();
	ama3D::Math::TVector3F vec2_2(0.0,1.0,0.0);
	BOOST_CHECK_FLOAT(vec2_1*vec2_2,cos(M_PI/4.0));
}
BOOST_AUTO_TEST_CASE(vec3_cross)
{
	ama3D::Math::TVector3F vec1_1(1.0,0.0,0.0);
	ama3D::Math::TVector3F vec1_2(0.0,1.0,0.0);
	ama3D::Math::TVector3F res1(0.0,0.0,1.0);
	BOOST_CHECK_EQUAL(vec1_1^vec1_2,res1);

	ama3D::Math::TVector3F vec2_1(1.0,1.0,0.0);
	vec2_1.Normalize();
	ama3D::Math::TVector3F vec2_2(0.0,1.0,0.0);
	BOOST_CHECK_FLOAT((vec2_1^vec2_2).z,sin(M_PI/4.0));
}
