#include <iostream>
#include <matrix/matrix.h>

int main(int, char**)
{
	{
		matrix::Matrix mtx = matrix::Matrix(0);

		//1
		for (int i = 0; i <= 9; ++i)
			mtx[i][i] = i;

		//2
		for (int r = 9, c = 0; r >= 0; --r, ++c)
			mtx[r][c] = r;

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

		//reset
		mtx[1][1] = 0;
	}

	//N-dimention
	{
		matrix::Matrix mtx = matrix::Matrix(0);
		mtx[1][2][3][4][5] = 7;
	}

	return 0;
}