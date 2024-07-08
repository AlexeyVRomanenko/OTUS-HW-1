#pragma once
#include <matrix/fwd.h>

namespace matrix
{
	class Matrix
	{
	public:
		using value_type = double;

		Matrix(value_type def_val);

		size_t count() const;



	private:
		class MatrixProxy;
		
		const std::unique_ptr<MatrixProxy> m_mtx;
	};
}