#pragma once
#include <memory>
#include <type_traits>

namespace my
{
	template <class T, class Allocator = std::allocator<T>>
	class vector
	{
	public:
		typedef vector<T, Allocator> this_type;
		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;

		vector()
		{
		}

		~vector()
		{
		}

		iterator begin() const
		{
			return m_data;
		}

		const_iterator cbegin() const
		{
			return m_data;
		}

		iterator end() const
		{
			return m_data + m_size;
		}

		const_iterator cend() const
		{
			return m_data + m_size;
		}

		void push_back(const T& x)
		{
			if (m_size == m_capacity)
			{
				const size_t old_size = size();
				const size_t new_size = old_size + 1;

				T* old_data = m_data;
				const size_t old_capacity = m_capacity;

				this_type new_vector;
				new_vector.alloc(m_capacity * 2 + 1);
				new_vector.copy_construct(new_vector.begin(), new_vector.begin() + old_size, begin());
				new_vector.copy_construct(new_vector.begin() + old_size, new_vector.begin() + new_size, &x);
				new_vector.m_size = new_size;
				swap(new_vector);

				{
					m_allocator.deallocate(old_data, old_capacity);
				}
			}
			else
			{
				++m_size;
				copy_construct(end() - 1, end(), &x);
			}
		}

		void swap(this_type& a)
		{
			std::swap(m_capacity, a.m_capacity);
			std::swap(m_size, a.m_size);
			std::swap(m_data, a.m_data);
		}

		bool empty() const
		{
			return m_size == 0;
		}

		size_t size() const
		{
			return m_size;
		}

	private:
		void alloc(const size_t& cap)
		{
			m_data = m_allocator.allocate(cap);
			m_capacity = cap;
		}

		template <typename src_iterator_t>
		void copy_construct(iterator dest_begin, const_iterator dest_end, src_iterator_t src_begin) const
		{
			iterator i_dest = dest_begin;
			src_iterator_t i_src = src_begin;
			for (; i_dest != dest_end; ++i_dest, ++i_src) {
				::new (i_dest) value_type(*i_src);
			}
		}

	private:
		std::size_t m_size = 0;
		std::size_t m_capacity = 0;
		T* m_data = nullptr;

		Allocator m_allocator;
	};
}