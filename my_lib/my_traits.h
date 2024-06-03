#pragma once
#include <iostream>
#include <type_traits>

namespace my
{
	template<typename Integer, typename = std::enable_if_t<std::is_integral<Integer>::value>>
	void print_ip(const Integer& val)
	{
		const uint8_t* arr = reinterpret_cast<const uint8_t*>(&val);
		for (size_t i = sizeof(val) - 1; i > 0; i--)
		{
			std::cout << (int)arr[i] << '.';
		}
		std::cout << (int)arr[0] << std::endl;
	};
}