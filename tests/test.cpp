#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

#include <map>
#include <string>

#include <my_allocator.h>

BOOST_AUTO_TEST_SUITE(TESTING)

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	{
		using int_str_map = std::map<int, std::string>;
		using int_str_map_my_alloc = std::map<int, std::string, std::less<int_str_map::key_type>, my::allocator_10<int_str_map::value_type>>;

		int_str_map_my_alloc my_map;

		my_map = { { 1, "1" }, { 2, "2" }, { 3, "3" }, { 4, "4" }, { 5, "5" } };

		for (int i = 6; i < 12; ++i)
		{
			if (i == 11)
			{
				BOOST_CHECK_THROW(my_map.insert({ i, std::to_string(i) }), std::bad_alloc);
			}
			else
			{
				BOOST_CHECK_NO_THROW(my_map.insert({ i, std::to_string(i) }));
			}			
		}
	}

	BOOST_TEST_MESSAGE("Tests OK.");
}

BOOST_AUTO_TEST_SUITE_END()