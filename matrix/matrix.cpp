#include "pch.h"
#include <matrix/matrix.h>

using namespace matrix;

using item_path_t = std::vector<Matrix::index_type>;

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

using matrix_t = std::unordered_map<item_path_t, Matrix::value_type>;

/////////////////////////////////////////////////////////////////

class matrix::MatrixProxy
{
public:
	MatrixProxy(Matrix::value_type def_val) :
		m_def_val(def_val)
	{
	}

	size_t size() const
	{
		return m_matrix.size();
	}

	Matrix::value_type get(const item_path_t& p) const
	{
		const auto i = m_matrix.find(p);
		if (i != m_matrix.end())
			return i->second;

		return m_def_val;
	}

	void set(const item_path_t& p, Matrix::value_type v)
	{
		m_matrix[p] = v;
	}

	void erase(const item_path_t& p)
	{
		m_matrix.erase(p);
	}

private:
	matrix_t m_matrix;

	const Matrix::value_type m_def_val = {};
};

//////////////////////////////////////////////////////////

class matrix::MatrixItemProxy
{
	friend matrix::MatrixProxy;

public:
	MatrixItemProxy(matrix::MatrixProxy* matrix, const item_path_t& path) :
		m_matrix(matrix),
		m_path(path)
	{
	}

	void set(Matrix::value_type v)
	{
		m_matrix->set(m_path, v);
	}

	Matrix::value_type get()
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
	m_matrix(new matrix::MatrixProxy(def_val))
{
}

Matrix::~Matrix()
{
	delete m_matrix;
}

size_t Matrix::size() const
{
	return m_matrix->size();
}

Matrix::Item Matrix::operator[](index_type i)
{
	Matrix::Item item(new matrix::MatrixItemProxy(m_matrix, item_path_t{ i }));
	return item;
}

////////////////////////////////////////////////////////////////////////////

Matrix::Item::Item(MatrixItemProxy* item) :
	m_item(item)
{
}

Matrix::Item::~Item()
{
	delete m_item;
}

Matrix::Item Matrix::Item::operator[](index_type i)
{
	item_path_t path = m_item->path();
	path.push_back(i);
	Matrix::Item item(new matrix::MatrixItemProxy(m_item->matrix(), path));
	return item;
}

void Matrix::Item::operator=(value_type v)
{
	m_item->set(v);
}

bool Matrix::Item::operator==(value_type v) const
{
	return m_item->get() == v;
}