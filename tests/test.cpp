#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <my_traits.h>

///////////////////////////////////////////////////////////

struct cout_redirect
{
	cout_redirect(std::streambuf* new_buffer)
		: old(std::cout.rdbuf(new_buffer))
	{
	}

	~cout_redirect()
	{
		std::cout.rdbuf(old);
	}

private:
	std::streambuf* old = nullptr;
};

void BOOST_CHECK_COUT(const std::function<void()>& fn, const char* ref_str)
{
	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		fn();
	}
	BOOST_CHECK(output.is_equal(ref_str));
}

///////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);
	//using namespace std::literals::string_literals;

	BOOST_CHECK_COUT([]() { my::print_ip(int8_t{ -1 }); }, "255\n");
	BOOST_CHECK_COUT([]() { my::print_ip(int16_t{ 0 }); }, "0.0\n");
	BOOST_CHECK_COUT([]() { my::print_ip(int32_t{ 2130706433 }); }, "127.0.0.1\n");
	BOOST_CHECK_COUT([]() { my::print_ip(int64_t{ 8875824491850138409 }); }, "123.45.67.89.101.112.131.41\n");
	//BOOST_CHECK_COUT([]() { my::print_ip("Hello, World!"s); }, "Hello, World!\n");
	BOOST_CHECK_COUT([]() { my::print_ip(std::vector<int>{100, 200, 300, 400}); }, "100.200.300.400\n");
	BOOST_CHECK_COUT([]() { my::print_ip(std::list<short>{400, 300, 200, 100}); }, "400.300.200.100\n");
	BOOST_CHECK_COUT([]() { my::print_ip(std::make_tuple(123, 456, 789, 0)); }, "123.456.789.0\n");

	BOOST_TEST_MESSAGE("Tests OK.");
}