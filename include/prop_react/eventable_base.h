#pragma once

#include <atomic>
#include <concepts>
#include <boost/container/small_vector.hpp>
#include <boost/signals2.hpp>
#include "scoped_connection.h"

namespace prop_react
{
	namespace detail
	{
		struct eventable
		{
		public:
			~eventable() = default;
		};
	}

	constexpr std::size_t preallocated_connections_count = 5;

	template<typename Derived>
	struct eventable_base : public detail::eventable
	{
		using self_type = eventable_base<Derived>;

		constexpr eventable_base() noexcept
		{}

		constexpr eventable_base(const self_type& rhs) noexcept
			: m_connections()
			, m_signal()
		{}

		constexpr eventable_base(self_type&& rhs) noexcept
			: m_connections(std::move(rhs.m_connections))
			, m_signal(std::move(rhs.m_signal))
		{
		}

		constexpr self_type& operator = (const self_type& rhs) noexcept
		{
			return *this;
		}

		constexpr self_type& operator = (self_type&& rhs) noexcept
		{
			m_connections = std::move(rhs.m_connections);
			m_signal = std::move(rhs.m_signal);

			return *this;
		}


		// connection management
		template<typename ...T>
		constexpr void add_dependency(const eventable_base<T>&... masters)
		{
			(this->_add_dependency(masters), ...);
		}

		template<typename T, std::invocable<const Derived&> F>
		constexpr void add_subscriber(const eventable_base<T>& slave_property, F callback) const
		{
			check_same_pointers(slave_property);
			slave_property.add_scoped_connection(m_signal.connect(std::move(callback)));
		}

		template<std::invocable<const Derived&> F>
		constexpr scoped_connection add_subscriber(F callback) const
		{
			return scoped_connection::create(std::move(m_signal.connect(std::move(callback))));
		}

		template<typename T, std::invocable<const Derived&, T&> F>
		constexpr void add_subscriber(const eventable_base<T>& slave_property, F callback) const
		{
			check_same_pointers(slave_property);

			slave_property.add_scoped_connection(
				std::move(
					m_signal.connect(
						[&slave_property, callback = std::move(callback)]
						(const Derived& derived)
						{
							callback(derived, static_cast<T&>(const_cast<eventable_base<T> &>(slave_property)));
						}
					)
				)
			);
		}

		constexpr void add_scoped_connection(boost::signals2::connection&& connection) const
		{
			m_connections.emplace_back(std::move(connection));
		}

	protected:
		constexpr void _fire_event() const
		{
			if (m_is_muted)
				m_was_muted_when_firing = true;
			else
			{
				bool exp = false;
				if (m_is_fired.compare_exchange_strong(exp, true))
				{
					m_signal(static_cast<const Derived&>(*this));
					m_is_fired = false;
				}
			}
		}
		constexpr void mute() const { m_is_muted = true; }
		constexpr void unmute() const { m_was_muted_when_firing = m_is_muted = false; }

		friend class scoped_lock;

		constexpr void _clear_connections() const
		{
			m_connections.clear();
		}

	private:
		mutable std::atomic<bool> m_is_fired = false;
		mutable boost::container::small_vector<boost::signals2::connection, preallocated_connections_count> m_connections;
		mutable boost::signals2::signal<void(const Derived&)> m_signal;

		mutable bool m_is_muted = false;
		mutable bool m_was_muted_when_firing = false;

	private:
		template<typename T>
		constexpr void _add_dependency(const eventable_base<T>& master_property)
		{
			check_same_pointers(master_property);
			master_property.add_subscriber(*this, [this](const auto&) { this->_fire_event(); });
		}

		template<typename T>
		constexpr void check_same_pointers(const eventable_base<T>& other) const
		{
			if constexpr (std::is_same_v<T, Derived>)
			{
				if (static_cast<const void*>(&other) == static_cast<const void*>(this))
					throw std::logic_error("Subscribing object on itself is wrong usage");
			}
		}
	};

}

namespace prop_react
{
	class scoped_lock
	{
	public:
		template <typename T>
		constexpr scoped_lock(eventable_base<T>& eventable)
			: m_holder(std::make_unique<eventable_holder<T>>(eventable))
		{}

	private:
		struct holder
		{
			virtual ~holder() = default;
		};

		template<typename T>
		struct eventable_holder : public holder
		{
			constexpr eventable_holder(eventable_base<T>& eventable)
				: eventable_ref(eventable)
			{
				eventable_ref.mute();
			}

			constexpr virtual ~eventable_holder()
			{
				if (!eventable_ref.m_was_muted_when_firing)
					eventable_ref.unmute();
				else
				{
					eventable_ref.unmute();
					eventable_ref._fire_event();
				}

			}

			eventable_base<T>& eventable_ref;
		};
	private:

		std::unique_ptr<holder> m_holder;
	};
}