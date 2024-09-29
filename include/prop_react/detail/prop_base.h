#pragma once

#include <prop_react/eventable_base.h>

#include <assert.h>
#include <concepts>
#include <exception>

#include "common_concepts.h"

namespace prop_react
{
	namespace detail
	{
		template<typename T>
		concept assignable_somehow =
			std::assignable_from<T&, T>
			|| std::assignable_from<T&, T&>
			|| std::assignable_from<T&, const T&>
			|| std::assignable_from<T&, T&&>
			|| std::assignable_from<T&, const T&&>;

		template<typename T>
		concept copy_constructible // use this concept because std::constructible_from checking std::move_constructible for some reason
			= std::constructible_from<T, T&>
			|| std::constructible_from<T, const T&>;
	}

	template<typename T>
	struct custom_equal : public std::conditional_t<std::equality_comparable<T>, std::true_type, std::false_type>
	{
		constexpr static bool equals(const T& lhs, const T& rhs)
		{
			return lhs == rhs;
		}
	};

	template<std::floating_point T>
	struct custom_equal<T> : public std::true_type
	{
		constexpr static bool equals(const T& lhs, const T& rhs)
		{
			return std::abs(rhs - lhs) < get_eps();
		}

	private:
		static constexpr T get_eps()
		{
			return std::numeric_limits<T>().epsilon();
		}
	};

	template<mpl::is_variant T>
	struct custom_equal<T> : public std::false_type
	{

	};

	template<typename _T>
	concept equality_comparable =
		custom_equal<_T>::value
		&& requires (const _T & val)
	{
		{ custom_equal<_T>::equals(val, val) } -> std::same_as<bool>;
	};

	template <typename F, typename Rx, typename ...Args>
	concept invocable_r = std::is_invocable_r_v<Rx, F, Args...>;
}

#define __CREATE_OPERATOR__(__op__) \
template<typename rhs_type, typename help = decltype(std::declval<T &>() __op__ std::declval<const rhs_type &>())> \
constexpr const T& operator __op__ (const rhs_type& rhs)	\
{															\
    m_val __op__ rhs;										\
    base_type::_fire_event();								\
    return m_val;											\
}

namespace prop_react
{
	namespace detail
	{
		template<typename property_type, typename T>
		class prop_base : public ::prop_react::eventable_base<property_type>
		{
		public:
			using base_type = ::prop_react::eventable_base<property_type>;
			using self_type = prop_base<property_type, T>;

			constexpr prop_base()
				: m_val()
			{}

			constexpr prop_base(const T& ref)
				: m_val(ref)
			{}

			constexpr prop_base(const self_type& rhs)
				: base_type(rhs)
				, m_val(rhs.m_val)
			{}

			constexpr prop_base(self_type&& rhs) noexcept
				: base_type(std::move(rhs))
				, m_val(std::move(rhs.m_val))
			{}

			constexpr prop_base(T&& rhs) noexcept
				: m_val(std::move(rhs))
			{}

			template<typename _T, std::invocable<const T&> F>
			constexpr void add_subscriber(const ::prop_react::eventable_base<_T>& slave_property, F callback) const
			{
				get_eventable_base().add_subscriber(slave_property, std::move([callback = std::move(callback)](const self_type& prop) { callback(prop.m_val); }));
			}

			template<typename _T, std::invocable<const T&, _T&> F>
			constexpr void add_subscriber(const eventable_base<_T>& slave_property, F callback) const
			{
				get_eventable_base().add_subscriber(slave_property,
					[&slave_property, callback = std::move(callback)]
					(const self_type& prop, _T& slave_prop)
					{
						callback(prop, static_cast<_T&>(const_cast<eventable_base<_T> &>(slave_property)));
					});
			}

			template<std::invocable<const T&> F>
			constexpr auto add_subscriber(F callback) const
			{
				return base_type::add_subscriber([callback = std::move(callback)](const self_type& prop) { callback(prop.m_val); });
			}

			constexpr const auto& get_eventable_base() const { return static_cast<const base_type&>(*this); }

			constexpr operator const T& () const& { return m_val; }

			constexpr const T& val() const
			{
				return m_val;
			}

			// some operators
			constexpr const T& operator = (const T& ref)
			{
				assign<const T&>(ref);
				return m_val;
			}

			constexpr T& operator = (T&& ref)
			{
				assign<T&&>(std::move(ref));
				return m_val;
			}

			constexpr self_type& operator = (self_type&& rhs)
			{
				m_val = std::move(rhs.m_val);
				static_cast<base_type&>(*this) = std::move(static_cast<base_type&>(rhs));
				return *this;
			}

			__CREATE_OPERATOR__(+= );
			__CREATE_OPERATOR__(-= );
			__CREATE_OPERATOR__(*= );
			__CREATE_OPERATOR__(/= );
			__CREATE_OPERATOR__(&= );
			__CREATE_OPERATOR__(|= );
			__CREATE_OPERATOR__(^= );

			std::shared_ptr<property_type> get_observable()
			{
				std::shared_ptr<property_type> observableProp = std::make_shared<property_type>();
				*observableProp = val();

				observableProp->get_eventable_base().add_subscriber(*this,
					[this](const property_type& prop)
					{
						*this = prop.val();
					});

				base_type::add_subscriber(*observableProp,
					[observableProp](const property_type& prop)
					{
						*observableProp = prop.val();
					});

				return observableProp;
			}

		protected:
			template<typename rhs_type, bool always_assign = false, bool always_fire = false>
			constexpr void assign(rhs_type rhs)
			{
				if (!static_cast<property_type&>(*this).is_assign_allowed(rhs))
					return;

				bool need_assign = always_assign;
				bool need_fire = always_fire;

				if constexpr (!always_assign || !always_fire)
				{
					bool are_equal = false;
					if constexpr (prop_react::equality_comparable<T>)
					{
						are_equal = custom_equal<T>::equals(m_val, rhs);
					}

					need_assign = need_assign || !are_equal;
					need_fire = need_fire || !are_equal;
				}

				if (need_assign)
				{
					if constexpr (std::is_move_assignable_v<T>)
						m_val = std::move(rhs);
					else
						m_val = rhs;
				}

				if (need_fire)
				{
					base_type::_fire_event();
				}
			}

			T m_val;
		};
	}
}

#undef __CREATE_OPERATOR__
