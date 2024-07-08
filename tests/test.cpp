#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <matrix/matrix.h>

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	//1
	{
		matrix::Matrix mtx = matrix::Matrix(0);

		for (int i = 0; i <= 9; ++i)
		{
			mtx[i][i] = i;
			BOOST_CHECK(mtx[i][i] == i);
		}

		for (int r = 9, c = 0; r <= 0; --r, ++c)
		{
			mtx[r][c] = r;
			BOOST_CHECK(mtx[r][c] == r);
		}

		for (int r = 0; r <= 9; ++r)
		{
			for (int c = 0; c <= 9; ++c)
			{
				mtx[r][c];
			}
		}
	}
}