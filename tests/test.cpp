#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TESTING)

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	BOOST_TEST_MESSAGE("Tests OK.");
}

BOOST_AUTO_TEST_SUITE_END()