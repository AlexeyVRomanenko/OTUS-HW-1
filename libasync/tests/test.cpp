#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);



}