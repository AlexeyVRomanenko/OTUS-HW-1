#define BOOST_TEST_MODULE test1
#include <lib.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test1)

BOOST_AUTO_TEST_CASE(test_case)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	BOOST_REQUIRE(lib::calc_sum(7, 8) == 15);
	BOOST_TEST_MESSAGE("Tests OK.");
}

BOOST_AUTO_TEST_SUITE_END()