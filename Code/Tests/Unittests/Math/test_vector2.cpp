#include <boost/test/unit_test.hpp>
#include <Math/Vector2.h>
#include <test_helper.h>

BOOST_AUTO_TEST_CASE(vec2_length)
{
	ama3D::Math::TVector2F vec(1.0,1.0);
	BOOST_CHECK_FLOAT(sqrt(2.0), vec.Length());
	BOOST_CHECK_EQUAL(2.0, vec.LengthSq());

	ama3D::Math::TVector2F vec2(2.0,1.0);
	BOOST_CHECK_FLOAT(sqrt(5.0), vec2.Length());
	BOOST_CHECK_EQUAL(5.0, vec2.LengthSq());
}
BOOST_AUTO_TEST_CASE(vec2_add)
{
	ama3D::Math::TVector2F vec(1.0,1.0);
	ama3D::Math::TVector2F resWait(2.0,2.0);
	ama3D::Math::TVector2F res = vec + vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(vec2_sub)
{
	ama3D::Math::TVector2F vec(1.0,1.0);
	ama3D::Math::TVector2F resWait(0.0,0.0);
	ama3D::Math::TVector2F res = vec - vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(vec2_normalize)
{
	ama3D::Math::TVector2F vec(10.0,5.0);
	vec.Normalize();
	BOOST_CHECK_EQUAL(vec.Length(), 1.0);
}

