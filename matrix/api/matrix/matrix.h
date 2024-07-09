#pragma once
#include <matrix/fwd.h>

namespace matrix
{
	class MatrixProxy;
	class MatrixItemProxy;

	using value_type = double;
	using index_type = uintmax_t;
	using item_path_t = std::vector<index_type>;

	class Item
	{
		friend class Matrix;

	public:
		Item operator[](index_type);
		void operator=(value_type);
		operator value_type ();

	private:
		Item(MatrixItemProxy*);
		std::shared_ptr<MatrixItemProxy> m_item;
	};

	////////////////////////////

	class Matrix
	{
	public:
		Matrix(value_type def_val);

		size_t size() const;
		Item operator[](index_type);
		void for_each(const std::function<void(const item_path_t&, value_type)>&);

	private:
		std::shared_ptr<MatrixProxy> m_matrix;
	};
}