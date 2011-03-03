#include <boost/test/unit_test.hpp>
#include <test_helper.h>
#include <System/MediaManager.h>
BOOST_AUTO_TEST_SUITE(test_mediamanager)
BOOST_AUTO_TEST_CASE(pathExist)
{
	BOOST_MESSAGE("pathExist test");
	MediaManager::Instance()->AddPath("../");
	BOOST_CHECK_EQUAL(MediaManager::Instance()->GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistNoSlash)
{
	BOOST_MESSAGE("pathExist with slash test");
	MediaManager::Instance()->AddPath("..");
	BOOST_CHECK_EQUAL(MediaManager::Instance()->GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistChilds)
{
	BOOST_MESSAGE("pathExist with child test");
	MediaManager::Instance()->AddPathAndChilds("..");
	BOOST_CHECK_GE(MediaManager::Instance()->GetPaths().size(),1);
}
BOOST_AUTO_TEST_CASE(pathNotExist)
{
	BOOST_MESSAGE("pathNotExist test");
	BOOST_CHECK_EXCEPTION(MediaManager::Instance()->AddPath("toto"), MediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_CASE(fileExist)
{
	BOOST_MESSAGE("fileExist test");
	MediaManager::Instance()->AddPath(".");
	BOOST_CHECK_EQUAL(MediaManager::Instance()->GetPath("TestAmaterasu3D.exe"),"../Build/TestAmaterasu3D.exe");
}
BOOST_AUTO_TEST_CASE(fileNotExist)
{
	BOOST_MESSAGE("fileNotExist test");
	MediaManager::Instance()->AddPathAndChilds("..");
	BOOST_CHECK_EXCEPTION(MediaManager::Instance()->GetPath("TOTO.exe"),MediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_SUITE_END()

