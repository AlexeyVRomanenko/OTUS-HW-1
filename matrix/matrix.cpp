#include "pch.h"
#include <matrix/matrix.h>

using namespace matrix;

Matrix::Matrix(value_type def_val) :
	m_mtx(new Matrix::MatrixProxy(def_val))
{
}

size_t Matrix::count() const
{
	return m_mtx->count();
}

class Matrix::MatrixProxy
{
public:
	Matrix::MatrixProxy(double def_val) :
		m_def_val(def_val)
	{
	}

	size_t count() const
	{
		
	}

private:
	using VAL
	using VALS = std::unordered_map<uintmax_t, double>;
	using MATRIX = std::unordered_map<uintmax_t, DIM_VALS>;
	MATRIX m_matrix;

	const double m_def_val = -1.0;
};

