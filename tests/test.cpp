#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

// <editor>

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
}