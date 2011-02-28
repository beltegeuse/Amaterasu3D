#include <boost/test/unit_test.hpp>
#include <test_helper.h>
#include <System/MediaManager.h>
BOOST_AUTO_TEST_SUITE(test_mediamanager)
BOOST_AUTO_TEST_CASE(pathExist)
{
	CMediaManager::AddPath("../");
	BOOST_CHECK_EQUAL(CMediaManager::GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistNoSlash)
{
	CMediaManager::AddPath("..");
	BOOST_CHECK_EQUAL(CMediaManager::GetPaths().front(),"../");
}
BOOST_AUTO_TEST_CASE(pathExistChilds)
{
	CMediaManager::AddPathAndChilds("..");
	BOOST_CHECK_GE(CMediaManager::GetPaths().size(),1);
}
BOOST_AUTO_TEST_CASE(pathNotExist)
{
	BOOST_CHECK_EXCEPTION(CMediaManager::AddPath("toto"), CMediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_CASE(fileExist)
{
	CMediaManager::AddPathAndChilds("..");
	BOOST_CHECK_EQUAL(CMediaManager::GetPath("TestAmaterasu3D.exe"),"../Build/TestAmaterasu3D.exe");
}
BOOST_AUTO_TEST_CASE(fileNotExist)
{
	CMediaManager::AddPathAndChilds("..");
	BOOST_CHECK_EXCEPTION(CMediaManager::GetPath("TOTO.exe"),CMediaManagerException, nullFuncException);
}
BOOST_AUTO_TEST_SUITE_END()

