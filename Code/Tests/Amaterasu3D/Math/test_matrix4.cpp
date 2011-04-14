#include <boost/test/unit_test.hpp>
#include <Math/Matrix4.h>
#include <test_helper.h>
#include <iostream>

BOOST_AUTO_TEST_CASE(mat4_inv_identity)
{
	Math::CMatrix4 matrix;
	matrix.Identity();

	BOOST_CHECK_EQUAL(matrix,matrix.Inverse());
}

BOOST_AUTO_TEST_CASE(mat4_inv1)
{
	Math::CMatrix4 matrixIdent;
	matrixIdent.Identity();

	Math::CMatrix4 matrix = Math::CMatrix4::CreatePerspectiveFOV(70,1.3333,1.0,40.0);

	BOOST_CHECK_EQUAL(matrixIdent,matrix*matrix.Inverse());
}

BOOST_AUTO_TEST_CASE(mat4_inv2)
{
	Math::CMatrix4 matrixIdent;
	matrixIdent.Identity();

	Math::CMatrix4 matrix;
	matrix.LookAt(Math::TVector3F(10,2,5), Math::TVector3F(0,1,0));

	BOOST_CHECK_EQUAL(matrixIdent,matrix*matrix.Inverse());
}

BOOST_AUTO_TEST_CASE(mat4_inv3)
{
	Math::CMatrix4 matrixIdent;
	matrixIdent.Identity();

	Math::CMatrix4 matrix;
	matrix.SetTranslation(10,20,3.14);

	BOOST_CHECK_EQUAL(Math::TVector3F(-10,-20,-3.14),matrix.Inverse().GetTranslation());
}

BOOST_AUTO_TEST_CASE(mat4_lookat)
{
	Math::CMatrix4 matrix;
	matrix.LookAt(Math::TVector3F(0,0,0), Math::TVector3F(0,1,0));

	Math::CMatrix4 matrix1;
	matrix.LookAt(Math::TVector3F(0,0,0), Math::TVector3F(0,-1,0));

	Math::CMatrix4 matrixRotation;
	matrixRotation.SetRotationY(M_PI/2.0);

	BOOST_CHECK_EQUAL(matrix,matrixRotation*matrix1);
}


