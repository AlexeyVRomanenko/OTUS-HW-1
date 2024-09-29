#pragma once

#include <prop_react/detail/prop_base.h>

namespace prop_react
{
	template<typename T>
	class property;

	namespace detail
	{

		template<typename property_type, typename T>
		class prop_struct : public prop_base<property_type, T>
		{
		public:
			using base_type = prop_base<property_type, T>;
			using self_type = prop_struct<property_type, T>;

			constexpr prop_struct() = default;

			constexpr prop_struct(const T& ref)
				: base_type(ref)
			{}

			constexpr prop_struct(const self_type& rhs) = default;

			constexpr prop_struct(self_type&& rhs) noexcept = default;

			constexpr prop_struct(T&& rhs) noexcept
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
				static_cast<base_type&>(*this) = std::move(static_cast<base_type&&>(rhs));

				return *this;
			}

			template<typename struct_field_t, invocable_r<const struct_field_t*, const T&> F>
			std::shared_ptr<::prop_react::property<struct_field_t>> get_observable_field_(F getFieldPtr)
			{
				using new_prop_type = ::prop_react::property<struct_field_t>;

				std::shared_ptr<new_prop_type> observableProp = std::make_shared<new_prop_type>();
				const struct_field_t* curValue = getFieldPtr(this->m_val);
				if (curValue)
					*observableProp = *curValue;

				observableProp->get_eventable_base().add_subscriber(std::as_const(*this),
					[this, getFieldPtr](const new_prop_type& newProp) {
						const struct_field_t* fieldPtr = getFieldPtr(this->m_val);
						if (fieldPtr)
						{
							if (*fieldPtr != newProp.val())
							{
								*const_cast<struct_field_t*>(fieldPtr) = newProp.val();
								this->_fire_event();
							}
						}
					});

				this->get_eventable_base().add_subscriber(*observableProp,
					[observablePropPtr = observableProp.get(), getFieldPtr](const auto& prop) {
						const struct_field_t* fieldPtr = getFieldPtr(prop);
						if (fieldPtr)
						{
							*observablePropPtr = *fieldPtr;
						}
					});

				return observableProp;
			}

			constexpr bool is_assign_allowed() const
			{
				return !m_var_ptr;
			}

			constexpr bool is_assign_allowed(const T& rhs) const
			{
				if (!is_assign_allowed())
					throw std::logic_error("cannot access when value moved to scoped holder with var method");
				return true;
			}

			class value_scoped_holder
				: public T
			{
				constexpr static bool should_copy = prop_react::detail::copy_constructible<T> && prop_react::equality_comparable<T> || !std::move_constructible<T>;

			public:
				value_scoped_holder() = delete;
				value_scoped_holder(const value_scoped_holder&) = delete;

				constexpr value_scoped_holder(property_type& prop) requires prop_react::detail::copy_constructible<T> || std::move_constructible<T> :
					T(static_cast<std::conditional_t<should_copy, const T&, T&&>>(prop.m_val)),
					m_prop(prop)
				{
					m_prop.m_var_ptr = this;
				}

				constexpr const T& operator =(const T& rhs) { static_cast<T&>(*this) = rhs; return *this; }

				constexpr ~value_scoped_holder()
				{
					m_prop.m_var_ptr = nullptr;
					m_prop.template assign<T&&, !should_copy, !should_copy>(static_cast<T&&>(*this));
				}

			private:
				property_type& m_prop;
			};

			constexpr const T& val() const { return (m_var_ptr) ? (*m_var_ptr) : (base_type::val()); }

			constexpr value_scoped_holder var()
			{
				if (is_assign_allowed())
				{
					return value_scoped_holder(static_cast<property_type&>(*this));
				}
				else
				{
					throw std::logic_error("cannot access when value moved to scoped holder with var method");
				}
			}

		private:

			T* m_var_ptr = nullptr;
		};

	}
}
