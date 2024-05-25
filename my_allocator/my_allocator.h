#pragma once

namespace my
{
	template <class T, unsigned int PREDEF_N>
	struct allocator
	{
		using value_type = T;

		allocator() noexcept
		{
		}

		template <class U>
		allocator(const allocator<U>&) noexcept
		{
		}

		T* allocate(std::size_t n)
		{
			return static_cast<T*>(::operator new(n * sizeof(T)));
		}

		void deallocate(T* p, std::size_t n)
		{
			::operator delete(p);
		}
	};

	template <class T, class U>
	constexpr bool operator== (const allocator<T>& a1, const allocator<U>& a2) noexcept
	{
		return true;
	}

	template <class T, class U>
	constexpr bool operator!= (const allocator<T>& a1, const allocator<U>& a2) noexcept
	{
		return false;
	}
}