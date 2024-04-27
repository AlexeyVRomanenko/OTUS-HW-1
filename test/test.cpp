#define BOOST_TEST_MODULE test1
#include <lib.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test1)

BOOST_AUTO_TEST_CASE(test_case)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_SUITE_END()