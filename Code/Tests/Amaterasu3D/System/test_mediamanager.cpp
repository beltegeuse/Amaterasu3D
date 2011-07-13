#include <boost/test/unit_test.hpp>
#include <test_helper.h>
#include <System/MediaManager.h>
BOOST_AUTO_TEST_SUITE(test_mediamanager)
BOOST_AUTO_TEST_CASE(pathExist)
{
	BOOST_MESSAGE("pathExist test");
	ama3D::MediaManager::Instance()->AddPath("../");
	BOOST_CHECK_EQUAL(ama3D::MediaManager::Instance()->GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistNoSlash)
{
	BOOST_MESSAGE("pathExist with slash test");
	ama3D::MediaManager::Instance()->AddPath("..");
	BOOST_CHECK_EQUAL(ama3D::MediaManager::Instance()->GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistChilds)
{
	BOOST_MESSAGE("pathExist with child test");
	ama3D::MediaManager::Instance()->AddPathAndChilds("..");
	BOOST_CHECK_GE(ama3D::MediaManager::Instance()->GetPaths().size(),1);
}
BOOST_AUTO_TEST_CASE(pathNotExist)
{
	BOOST_MESSAGE("pathNotExist test");
	BOOST_CHECK_EXCEPTION(ama3D::MediaManager::Instance()->AddPath("toto"), ama3D::MediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_CASE(fileExist)
{
	BOOST_MESSAGE("fileExist test");
	ama3D::MediaManager::Instance()->AddPath(".");
	BOOST_CHECK_EQUAL(ama3D::MediaManager::Instance()->GetPath("TestAmaterasu3D.exe"),"../Build/TestAmaterasu3D.exe");
}
BOOST_AUTO_TEST_CASE(fileNotExist)
{
	BOOST_MESSAGE("fileNotExist test");
	ama3D::MediaManager::Instance()->AddPathAndChilds("..");
	BOOST_CHECK_EXCEPTION(ama3D::MediaManager::Instance()->GetPath("TOTO.exe"),ama3D::MediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_SUITE_END()

