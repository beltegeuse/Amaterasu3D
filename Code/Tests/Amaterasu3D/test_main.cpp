#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE amaterasu3D
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test)
BOOST_AUTO_TEST_CASE(test_module)
{
	BOOST_CHECK_EQUAL(true, true);
}
BOOST_AUTO_TEST_SUITE_END()
