#include <boost/test/unit_test.hpp>
#include <Math/Quaternion.h>
#include <test_helper.h>

BOOST_AUTO_TEST_CASE(quaternion_3Dvector)
{
	Math::CQuaternion q;

	// Positif
	q.From3DVector(Math::TVector3F(1,0,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(1,0,0));
	q.From3DVector(Math::TVector3F(0,1,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(0,1,0));
	q.From3DVector(Math::TVector3F(0,0,1));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(0,0,1));

	// Negatif
	q.From3DVector(Math::TVector3F(-1,0,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(-1,0,0));
	q.From3DVector(Math::TVector3F(0,-1,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(0,-1,0));
	q.From3DVector(Math::TVector3F(0,0,-1));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(Math::TVector3F(1,0,0),0), Math::TVector3F(0,0,-1));
}
