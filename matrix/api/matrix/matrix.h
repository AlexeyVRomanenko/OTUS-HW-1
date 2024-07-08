#pragma once
#include <matrix/fwd.h>

namespace matrix
{
	class MatrixProxy;
	class MatrixItemProxy;

	class Matrix
	{
	public:
		using value_type = double;
		using index_type = uintmax_t;

		Matrix(value_type def_val);
		~Matrix();

		size_t size() const;

		struct Item
		{
			Item(MatrixItemProxy*);
			~Item();

			Item operator[](index_type);
			void operator=(value_type);
			bool operator==(value_type) const;

		private:
			MatrixItemProxy* m_item = nullptr;
		};

		Item operator[](index_type);

	private:
		MatrixProxy* m_matrix = nullptr;
	};
}