#pragma once

#include <prop_react/detail/prop_simple.h>
#include <prop_react/detail/prop_struct.h>

namespace prop_react
{
	namespace detail
	{
		template<typename To, typename From>
		concept convertible_from = std::convertible_to<From, To>;

		template<typename type_to_convert, typename property_type>
		concept can_be_converted_to = (sizeof(type_to_convert) == sizeof(property_type)) && convertible_from<type_to_convert, property_type>;
	}

	template<typename T>
	class property
		: public std::conditional_t < !std::is_class_v<T>, detail::prop_simple<property<T>, T>, detail::prop_struct<property<T>, T> >
	{
		static_assert(detail::assignable_somehow<T>);

	public:
		using base_type = std::conditional_t < !std::is_class_v<T>, detail::prop_simple<property<T>, T>, detail::prop_struct<property<T>, T> >;
		using self_type = property<T>;

		constexpr property() = default;

		constexpr property(const T& ref)
			: base_type(ref)
		{}

		constexpr property(const self_type& rhs) = default;

		constexpr property(self_type&& rhs) noexcept = default;

		constexpr property(T&& rhs) noexcept
			: base_type(std::move(rhs))
		{}

		constexpr const T& operator = (const T& ref)
		{
			return base_type::operator=(ref);
		}

		constexpr T& operator = (T&& ref)
		{
			return base_type::operator=(std::move(ref));
		}

		constexpr self_type& operator = (self_type&& rhs)
		{
			static_cast<base_type&>(*this) = std::move(static_cast<base_type&>(rhs));
			return *this;
		}

		template<detail::can_be_converted_to<T> _T>
		constexpr operator const property<_T>& () const& { return *reinterpret_cast<const property<_T>*>(this); }

		template<detail::can_be_converted_to<T> _T>
		constexpr operator property<_T>& ()& { return *reinterpret_cast<property<_T>*>(this); }
	};
}

namespace prop_react
{
	namespace detail
	{
		template<typename T1, typename T2>
		concept assignable_from =
			requires(T1 & lhs, const T2 && rhs)
		{
			{ lhs = rhs };
		}&&
			requires(T2& lhs, const T1&& rhs)
		{
			{ lhs = rhs };
		};
	}
	// after this call p2 become equal to p1
	template<typename T1, detail::assignable_from<T1> T2>
	constexpr void bind_equal(const ::prop_react::property<T1>& p1, ::prop_react::property<T2>& p2)
	{
		p2 = p1.val();

		p1.get_eventable_base().add_subscriber(p2, [&p2](const ::prop_react::property<T1>& _p1)
			{ p2 = _p1.val(); }
		);
		p2.get_eventable_base().add_subscriber(p1, [&p1](const ::prop_react::property<T2>& _p2)
			{ const_cast<::prop_react::property<T1>&>(p1) = _p2.val(); }
		);
	}
}