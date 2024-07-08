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
	public:
		Item(MatrixItemProxy*);
		~Item();

		Item operator[](index_type);
		void operator=(value_type);
		operator value_type ();

		item_path_t pos() const;

	private:
		std::shared_ptr<MatrixItemProxy> m_item;
	};

	////////////////////////////

	class Matrix
	{
	public:
		Matrix(value_type def_val);
		~Matrix();

		size_t size() const;

		Item operator[](index_type);

		std::vector<Item> GetItems() const;

	private:
		std::shared_ptr<MatrixProxy> m_matrix;
	};
}