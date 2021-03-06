#ifndef TEST_HELPER_H_
#define TEST_HELPER_H_
#include <cmath>
#include <boost/test/unit_test.hpp>
#include <Debug/Exceptions.h>

bool nullFuncException(const ama3D::CException& e);

#define BOOST_CHECK_FLOAT(v,e) BOOST_CHECK_LE(fabs(v-e), 0.0001);

#endif /* TEST_HELPER_H_ */
