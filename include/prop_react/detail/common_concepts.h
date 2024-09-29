#pragma once

#include <boost/variant.hpp>
#include <variant>

namespace mpl
{
	template <class... T>
	struct variant_traits : std::false_type {};

	template <class... T>
	struct variant_traits<std::variant<T...>>
		: std::true_type
	{
		using value_type = std::variant<T...>;
		static int index(const value_type& v) { return v.index(); }
	};

	template <class... T>
	struct variant_traits<boost::variant<T...>>
		: std::true_type
	{
		using value_type = boost::variant<T...>;
		static int index(const value_type& v) { return v.which(); }
	};

	template <typename T>
	concept is_variant = mpl::variant_traits<T>::value;
}