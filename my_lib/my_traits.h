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

	//template<class String, typename std::enable_if_t<std::is_same_v<String, std::string>, bool> = true>
	template<class String, typename std::enable_if_t<std::is_convertible_v<String, std::string>, bool> = true>
	void print_ip(const String& val)
	{
		std::cout << val << std::endl;
	}

	template <class T, template <class...> class Template>
	struct is_specialization_of : std::false_type {};

	template <template <class...> class Template, class... Args>
	struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

	constexpr bool b = is_specialization_of<std::tuple<int, double, std::string>, std::tuple>::value;
}