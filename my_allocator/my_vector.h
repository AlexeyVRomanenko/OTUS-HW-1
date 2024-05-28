#pragma once
#include <memory>

namespace my
{
	template <class T, class Allocator = std::allocator<T>>
	class vector
	{
	public:
		vector()
		{

		}

		void push_back(const T& x)
		{
			if (m_size == m_capacity)
			{
				const size_t old_capacity = m_capacity;

				m_capacity = m_capacity * 2 + 1;

				T* new_data = m_allocator.allocate(m_capacity);
				for (size_t i = 0; i <= m_size; ++i)
				{
					if (i == m_size)
						new_data[i] = x;
					else
						new_data[i] = m_data[i];
				}

				m_allocator.deallocate(m_data, old_capacity);

				m_data = new_data;
			}

			//m_allocator.construct(m_data + m_size * sizeof(T), x);
			++m_size;
		}

		bool empty() const
		{
			return m_size == 0;
		}

		size_t size() const
		{
			return m_size;
		}

		template <class T>
		class iterator
		{
		public:
			using value_type = T;
			using iterator_category = std::output_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using reference = T&;
			using const_reference = const reference;

			iterator() noexcept = default;
			iterator(const iterator& other) noexcept = default;
			iterator& operator=(const iterator& other) noexcept = default;
			iterator(value_type* ptr) noexcept : m_ptr(ptr) {}

			reference operator*() noexcept {
				return *m_ptr;
			}

			const_reference operator*() const noexcept {
				return *m_ptr;
			}

			iterator& operator++() noexcept {
				++m_ptr;
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator temp = *this;
				++(*this);
				return temp;
			}

		private:
			value_type* m_ptr = nullptr;
		};

	private:
		std::size_t m_size = 0;
		std::size_t m_capacity = 0;
		T* m_data = nullptr;

		Allocator m_allocator;
	};
}