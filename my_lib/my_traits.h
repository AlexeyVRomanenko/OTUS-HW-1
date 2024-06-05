#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>
#include <iterator>
#include <xutility>

namespace my
{
	//integer
	template<class T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
	void print_ip(T val)
	{
		const uint8_t* arr = reinterpret_cast<const uint8_t*>(&val);
		for (size_t i = sizeof(val) - 1; i > 0; i--)
		{
			std::cout << (int)arr[i] << '.';
		}
		std::cout << (int)arr[0] << std::endl;
	};

	//string
	template<class T, typename std::enable_if_t<std::is_convertible_v<T, std::string>, bool> = true>
	void print_ip(T&& val)
	{
		std::cout << val << std::endl;
	}

	//vector && list
	template<typename Test, template<typename...> class Ref>
	struct is_specialization : std::false_type {};
	template<template<typename...> class Ref, typename... Args>
	struct is_specialization<Ref<Args...>, Ref> : std::true_type {};
	template<class T>
	constexpr bool is_vector = is_specialization<T, std::vector>::value;
	template<class T>
	constexpr bool is_list = is_specialization<T, std::list>::value;

	template<typename T, typename std::enable_if_t<is_vector<T> || is_list<T>, bool> = true, typename ...Args>
	void print_ip(T&& v)
	{
		if constexpr (std::random_access_iterator<decltype(v.cbegin())>)
		{
			for (auto it = v.cbegin(); it != v.cend(); ++it)
			{
				if (it == v.cend() - 1)
					std::cout << *it << std::endl;

				else
					std::cout << *it << '.';
			}
		}
		else
		{
			for (const auto& i : v)
			{
				if (&i == &v.back())
					std::cout << i << std::endl;
				else
					std::cout << i << '.';
			}
		}
	}

	/*template <class T>
	struct is_vector : std::false_type {};
	template <class ...Args>
	struct is_vector<std::vector<Args...>> : std::true_type {};
	template <class T>
	constexpr bool is_vector_v = is_vector<T>::value;
	template <class T>
	struct is_list : std::false_type {};
	template <class ...Args>
	struct is_list<std::list<Args...>> : std::true_type {};
	template <class T>
	constexpr bool is_list_v = is_list<T>::value;
	template<class T, typename std::enable_if_t<is_list_v<T> || is_vector_v<T>, bool> = true>
	void print_ip(T&& val)
	{
		for (const auto& i : val)
		{
			if (&i == &val.back())
				std::cout << i << std::endl;
			else
				std::cout << i << '.';
		}
	}*/

	//tuple (magic)
	template <class Type, class ...Args>
	struct check_type : std::false_type {};
	template <class Type>
	struct check_type <Type> : std::true_type {};
	template <class Type, class ...Args>
	struct check_type<Type, Type, Args...>
	{
		constexpr static bool value = check_type<Type, Args...>::value;
	};
	template <class T>
	struct is_one_type_tuple : std::false_type {};
	template <>
	struct is_one_type_tuple<std::tuple<>> : std::true_type {};
	template <class T, class ...Args>
	struct is_one_type_tuple<std::tuple<T, Args...>> : check_type<T, Args...> {};
	template <class T>
	constexpr bool is_one_type_tuple_v = is_one_type_tuple<T>::value;

	template<class T, typename std::enable_if_t<is_one_type_tuple_v<T>, bool> = true>
	void print_ip(T&& val)
	{
		std::stringstream sstrm;
		std::apply([&sstrm](const auto& ...arg) { ((sstrm << arg << '.'), ...); }, val);
		std::cout << sstrm.str() << std::endl;
	}
}