#define BOOST_TEST_MODULE test

//ip_parser
#include <ip_parser.h>
//boost
#include <boost/test/unit_test.hpp>
//std
#include <locale>
#include <codecvt>

BOOST_AUTO_TEST_SUITE(test1)

BOOST_AUTO_TEST_CASE(test_case)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	ip_parser::ip_t ip;
	std::string str = R"(0.255.23.1 \t text2 \t text3)";
	BOOST_REQUIRE_MESSAGE(ip_parser::parse(str, ip), str.c_str());

	BOOST_TEST_MESSAGE("Tests OK.");
}

BOOST_AUTO_TEST_SUITE_END()