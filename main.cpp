#include <iostream>
#include <string>
#include <libasync.h>

constexpr const uint32_t DEF_N = 3;

int main(int ac, char** av)
{
	uint32_t N = DEF_N;

	if (ac == 2)
	{
		try
		{
			N = std::stoul(av[1]);
		}
		catch (...)
		{
			N = DEF_N;
		}
	}

	const libasync::context_t c = libasync::connect(N);

	std::string buffer(1e6, 0);
	libasync::receive(c, buffer.data(), buffer.size());

	libasync::disconnect(c);

	return 0;
}