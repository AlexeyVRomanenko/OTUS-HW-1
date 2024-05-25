#pragma once

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

		static const uint32_t PRELOC_N = 10;

		allocator_10() noexcept :
			m_pool(::operator new (sizeof(uint8_t)* PRELOC_N), deleter()),
			m_pool_elems(PRELOC_N)
		{
		}

		template <class U>
		allocator_10(const allocator_10<U>& a) noexcept
		{
			m_pool = a.m_pool;
			m_pool_elems = a.m_pool_elems;
		}

		T* allocate(std::size_t n)
		{
			if ((m_user_elems + n) > m_pool_elems)
			{
				throw std::bad_alloc();
				return nullptr;
			}

			m_user_elems += n;

			return static_cast<T*>(::operator new(n * sizeof(T)));
		}

		void deallocate(T* p, std::size_t n)
		{
			
		}

		//initial pool
		std::shared_ptr<void> m_pool;
		std::size_t m_pool_elems = 0;

		//user
		std::size_t m_user_elems = -1; //1 - undefined map constructor element

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