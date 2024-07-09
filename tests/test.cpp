#define BOOST_TEST_MODULE test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <format>
#include <matrix/matrix.h>

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);


	{
		matrix::Matrix mtx = matrix::Matrix(0);

		//1
		for (int i = 0; i <= 9; ++i)
		{
			mtx[i][i] = i;
			BOOST_CHECK(mtx[i][i] == i);
		}
		BOOST_CHECK(mtx.size() == 9);

		//2
		for (int r = 9, c = 0; r >= 0; --r, ++c)
		{
			mtx[r][c] = r;
			BOOST_CHECK(mtx[r][c] == r);
		}
		BOOST_CHECK(mtx.size() == 18);

		//3
		for (int r = 1; r <= 8; ++r)
		{
			for (int c = 1; c <= 8; ++c)
				std::cout << mtx[r][c] << ' ';

			std::cout << std::endl;
		}

		//4
		std::cout << mtx.size() << std::endl;

		//5
		for (int r = 0; r <= 9; ++r)
		{
			for (int c = 0; c <= 9; ++c)
				std::cout << "[" << r << ";" << c << "]=" << mtx[r][c] << '\t';

			std::cout << std::endl;
		}

		//reset
		mtx[1][1] = 0;
		BOOST_CHECK(mtx.size() == 17);

		//all items
		mtx.for_each([](const matrix::item_path_t& path, matrix::value_type value)
			{
				std::cout << '[';
				for (const auto& p : path)
				{
					std::cout << p;
					if (&p != &path.back())
						std::cout << ';';
				}
				std::cout << ']';
				std::cout << '=' << value;
				std::cout << std::endl;
			});
	}

	//N-dimention
	{
		matrix::Matrix mtx = matrix::Matrix(0);

		mtx[1][2][3][4][5] = 7;
		BOOST_CHECK(mtx[1][2][3][4][5] == 7);
		BOOST_CHECK(mtx[0][0] == 0);
		BOOST_CHECK(mtx[3][1] == 0);
	}
}