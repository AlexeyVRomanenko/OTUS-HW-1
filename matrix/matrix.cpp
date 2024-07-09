#include "pch.h"
#include <matrix/matrix.h>

using namespace matrix;

template <>
struct std::hash<item_path_t>
{
	std::size_t operator()(const item_path_t& k) const
	{
		size_t seed = 0;
		for (const auto& i : k)
			std::hash_combine(seed, i);

		return seed;
	}
};

using matrix_t = std::unordered_map<item_path_t, value_type>;

/////////////////////////////////////////////////////////////////

class matrix::MatrixProxy
{
public:
	MatrixProxy(value_type def_val) :
		m_def_val(def_val)
	{
	}

	size_t size() const
	{
		return m_matrix.size();
	}

	value_type get(const item_path_t& p) const
	{
		const auto i = m_matrix.find(p);
		if (i != m_matrix.end())
			return i->second;

		return m_def_val;
	}

	void set(const item_path_t& p, value_type v)
	{
		if (v == m_def_val)
			m_matrix.erase(p);
		else
			m_matrix[p] = v;
	}

	std::vector<item_path_t> get_all() const
	{
		std::vector<item_path_t> items;
		items.reserve(m_matrix.size());
		for (const auto& i : m_matrix)
			items.push_back(i.first);

		return items;
	}

private:
	matrix_t m_matrix;

	const value_type m_def_val = {};
};

//////////////////////////////////////////////////////////

class matrix::MatrixItemProxy
{
public:
	MatrixItemProxy(matrix::MatrixProxy* matrix, const item_path_t& path) :
		m_matrix(matrix),
		m_path(path)
	{
	}

	void set(value_type v)
	{
		m_matrix->set(m_path, v);
	}

	value_type get()
	{
		return m_matrix->get(m_path);
	}

	matrix::MatrixProxy* matrix() const { return m_matrix; }
	const item_path_t& path() const { return m_path; }

private:
	const item_path_t m_path;
	matrix::MatrixProxy* m_matrix = nullptr;
};

////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(value_type def_val) :
	m_matrix(std::make_shared<matrix::MatrixProxy>(def_val))
{
}

size_t Matrix::size() const
{
	return m_matrix->size();
}

Item Matrix::operator[](index_type i)
{
	Item item(new matrix::MatrixItemProxy(m_matrix.get(), item_path_t{i}));
	return item;
}

void Matrix::for_each(const std::function<void(const item_path_t&, value_type)>& fn)
{
	for (const auto& i : m_matrix->get_all())
		fn(i, m_matrix->get(i));
}

////////////////////////////////////////////////////////////////////////////

Item::Item(MatrixItemProxy* item) :
	m_item(item)
{
}

Item Item::operator[](index_type i)
{
	item_path_t path = m_item->path();
	path.push_back(i);
	Item item(new matrix::MatrixItemProxy(m_item->matrix(), path));
	return item;
}

void Item::operator=(value_type v)
{
	m_item->set(v);
}

Item::operator value_type ()
{
	return m_item->get();
}