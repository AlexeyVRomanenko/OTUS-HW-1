#include <iostream>
#include <my_traits.h>

/**
 * \brief MY DOXY Doc
 *  111222
 */

int main(int, char**)
{
	my::print_ip(int8_t{ -1 });
	my::print_ip(int16_t{ 0 });
	my::print_ip(int32_t{ 2130706433 });
	my::print_ip(int64_t{ 8875824491850138409 });
	my::print_ip("Hello, World!");
	my::print_ip(std::vector<int>{100, 200, 300, 400});
	my::print_ip(std::list<short>{400, 300, 200, 100});
	my::print_ip(std::make_tuple(123, 456, 789, 0));

	return 0;
}