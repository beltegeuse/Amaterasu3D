#include <boost/test/unit_test.hpp>
#include <Math/Vector3.h>
#include <test_helper.h>
BOOST_AUTO_TEST_SUITE(test_vector3)
BOOST_AUTO_TEST_CASE(length)
{
	Math::TVector3F vec(1.0,1.0,1.0);
	BOOST_CHECK_FLOAT(sqrt(3.0), vec.Length());
	BOOST_CHECK_EQUAL(3.0, vec.LengthSq());

	Math::TVector3F vec2(2.0,1.0,0.0);
	BOOST_CHECK_FLOAT(sqrt(5.0), vec2.Length());
	BOOST_CHECK_EQUAL(5.0, vec2.LengthSq());
}
BOOST_AUTO_TEST_CASE(add)
{
	Math::TVector3F vec(1.0,1.0,0.0);
	Math::TVector3F resWait(2.0,2.0,0.0);
	Math::TVector3F res = vec + vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(sub)
{
	Math::TVector3F vec(1.0,1.0,-1.0);
	Math::TVector3F resWait(0.0,0.0,0.0);
	Math::TVector3F res = vec - vec;
	BOOST_CHECK_EQUAL(resWait, res);
}
BOOST_AUTO_TEST_CASE(normalize)
{
	Math::TVector3F vec(10.0,5.0,1.0);
	vec.Normalize();
	BOOST_CHECK_EQUAL(vec.Length(), 1.0);
}
BOOST_AUTO_TEST_CASE(dot)
{
	Math::TVector3F vec1_1(1.0,0.0,0.0);
	Math::TVector3F vec1_2(0.0,1.0,0.0);
	BOOST_CHECK_EQUAL(vec1_1*vec1_2,0.0);

	Math::TVector3F vec2_1(1.0,1.0,0.0);
	vec2_1.Normalize();
	Math::TVector3F vec2_2(0.0,1.0,0.0);
	BOOST_CHECK_FLOAT(vec2_1*vec2_2,cos(M_PI/4.0));
}
BOOST_AUTO_TEST_CASE(cross)
{
	Math::TVector3F vec1_1(1.0,0.0,0.0);
	Math::TVector3F vec1_2(0.0,1.0,0.0);
	Math::TVector3F res1(0.0,0.0,1.0);
	BOOST_CHECK_EQUAL(vec1_1^vec1_2,res1);

	Math::TVector3F vec2_1(1.0,1.0,0.0);
	vec2_1.Normalize();
	Math::TVector3F vec2_2(0.0,1.0,0.0);
	BOOST_CHECK_FLOAT((vec2_1^vec2_2).z,sin(M_PI/4.0));
}
BOOST_AUTO_TEST_SUITE_END()
