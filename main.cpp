#include <iostream>
#include <string>
#include <front/front.h>
#include <back/back.h>

constexpr const uint32_t DEF_N = 3;

int main(int ac, char** av)
{
	uint32_t N = {};
	if (ac == 1)
	{
		try
		{
			N = std::stoul(av[0]);
		}
		catch (...)
		{
			N = DEF_N;
		}
	}

	std::cout << N << std::endl;

	std::shared_ptr<front::IFront> front = front::CreateFront();
	std::shared_ptr<back::IBack> back = back::CreateBack(front, N);

	front->Run();

	return 0;
}