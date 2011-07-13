#include <boost/test/unit_test.hpp>
#include <Math/Quaternion.h>
#include <test_helper.h>

BOOST_AUTO_TEST_CASE(quaternion_3Dvector)
{
	ama3D::Math::CQuaternion q;

	// Positif
	q.From3DVector(ama3D::Math::TVector3F(1,0,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0),ama3D::Math::TVector3F(1,0,0));
	q.From3DVector(ama3D::Math::TVector3F(0,1,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0), ama3D::Math::TVector3F(0,1,0));
	q.From3DVector(ama3D::Math::TVector3F(0,0,1));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0), ama3D::Math::TVector3F(0,0,1));

	// Negatif
	q.From3DVector(ama3D::Math::TVector3F(-1,0,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0), ama3D::Math::TVector3F(-1,0,0));
	q.From3DVector(ama3D::Math::TVector3F(0,-1,0));
	BOOST_CHECK_EQUAL(q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0), ama3D::Math::TVector3F(0,-1,0));
	q.From3DVector(ama3D::Math::TVector3F(0,0.2,-0.98));

	// FIXME: Correct Quaternion
	ama3D::Math::TVector3F v(0,0.2,-0.98);
	v.Normalize();
	ama3D::Math::TVector3F res = q.ToMatrix().Transform(ama3D::Math::TVector3F(1,0,0),0);
	res.Normalize();
	BOOST_CHECK_EQUAL(res, v);
}
