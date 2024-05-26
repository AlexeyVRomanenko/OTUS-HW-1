#pragma once

#include <memory>
#include <assert.h>

namespace my
{
	struct deleter
	{
		void operator()(void* ptr)
		{
			::operator delete(ptr);
		}
	};

	template <class T>
	struct allocator_10
	{
		using value_type = T;

		//options
		struct opts {
			static constexpr uint32_t preloc_n = 10;
			static constexpr bool extandable = false;
			static constexpr uint32_t ext_n = 5;
		};

		//initial pool
		std::shared_ptr<void> m_pre_pool;
		std::size_t m_pre_pool_n = 0;

		//user
		std::shared_ptr<void> m_user_pool;
		std::size_t m_user_pool_n = -1; //1 - undefined map constructor element

		//state
		std::size_t m_cmn_n = 0;

		allocator_10() noexcept :
			m_pre_pool(::operator new (sizeof(uint8_t)* opts::preloc_n), deleter()),
			m_pre_pool_n(opts::preloc_n)
		{
		}

		template <class U>
		allocator_10(const allocator_10<U>& a) noexcept
		{
			m_pre_pool = a.m_pre_pool;
			m_pre_pool_n = a.m_pre_pool_n;
			m_user_pool = a.m_user_pool;
			m_user_pool_n = a.m_user_pool_n;
			m_cmn_n = a.m_cmn_n;
		}

		T* allocate(std::size_t n)
		{
			if ((m_cmn_n + n) > m_pre_pool_n)
			{
				if (opts::extandable)
				{
					if (m_user_pool)
					{
						assert(m_user_pool_n == 0);

						std::shared_ptr<void> new_pool(::operator new (sizeof(uint8_t) * (m_user_pool_n + opts::ext_n)), deleter());
						memcpy(new_pool.get(), m_user_pool.get(), sizeof(sizeof(uint8_t) * (m_user_pool_n)));
						m_user_pool = new_pool;
						m_user_pool_n += opts::ext_n;
					}
					else
					{
						m_user_pool.reset(::operator new (sizeof(uint8_t) * (m_user_pool_n + opts::ext_n)));
						m_user_pool_n += opts::ext_n;
					}
				}
				else
				{
					throw std::bad_alloc();
					return nullptr;
				}
			}

			m_cmn_n += n;

			return static_cast<T*>(::operator new(n * sizeof(T)));
		}

		void deallocate(T* p, std::size_t n)
		{

		}

		/*using propagate_on_container_copy_assignment = std::true_type;
		using propagate_on_container_move_assignment = std::true_type;
		using propagate_on_container_swap = std::true_type;*/
	};

	template <class T, class U>
	constexpr bool operator== (const allocator_10<T>& a1, const allocator_10<U>& a2) noexcept
	{
		return true;
	}

	template <class T, class U, unsigned int PREDEF_N>
	constexpr bool operator!= (const allocator_10<T>& a1, const allocator_10<U>& a2) noexcept
	{
		return false;
	}
}