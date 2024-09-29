#pragma once

#include <prop_react/detail/prop_base.h>

namespace prop_react
{
	namespace detail
	{

		template<typename property_type, typename T>
		class prop_simple : public prop_base<property_type, T>
		{
		public:
			using base_type = prop_base<property_type, T>;
			using self_type = prop_simple<property_type, T>;

			constexpr prop_simple() = default;

			constexpr prop_simple(const T& ref)
				: base_type(ref)
			{}

			constexpr prop_simple(const self_type& rhs) = default;

			constexpr prop_simple(self_type&& rhs) noexcept = default;

			constexpr prop_simple(T&& rhs) noexcept
				: base_type(std::move(rhs))
			{}

			// some operators
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

			constexpr bool is_assign_allowed(const T& rhs) const
			{
				return true;
			}
		};

	}
}