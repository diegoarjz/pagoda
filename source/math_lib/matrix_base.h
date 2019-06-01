#ifndef SELECTOR_MATH_LIB_MATRIX_BASE_H_
#define SELECTOR_MATH_LIB_MATRIX_BASE_H_

#include <common/assertions.h>
#include <common/profiler.h>

#include "vec_base.h"

namespace selector
{
template<int NumCols, int NumRows, class Rep>
class MatrixBase
{
public:
	using MatrixType = MatrixBase<NumCols, NumRows, Rep>;
	using ColumnType = VecBase<NumRows, Rep>;
	using RowType = VecBase<NumCols, Rep>;
	static const size_t kNumCols = NumCols;
	static const size_t kNumRows = NumRows;
	static const size_t kSizeInBytes = kNumCols * kNumRows * sizeof(Rep);

	MatrixBase()
	{
		START_PROFILE;
		for (auto r = 0; r < NumRows; ++r)
		{
			for (auto c = 0; c < NumCols; ++c)
			{
				m_elems[r * NumCols + c] = 0;
			}
		}
	}

	explicit MatrixBase(const Rep &diagonal)
	{
		START_PROFILE;
		for (auto r = 0; r < NumRows; ++r)
		{
			for (auto c = 0; c < NumCols; ++c)
			{
				m_elems[r * NumCols + c] = (r == c ? diagonal : 0);
			}
		}
	}

	ColumnType Col(const size_t &col) const
	{
		START_PROFILE;
		CRITICAL_ASSERT(col < NumCols);

		ColumnType c;
		for (auto i = 0u; i < NumRows; ++i)
		{
			c[i] = m_elems[col + NumCols * i];
		}

		return c;
	}

	RowType Row(const size_t &row) const
	{
		START_PROFILE;
		CRITICAL_ASSERT(row < NumRows);

		RowType r;
		for (auto i = 0u; i < NumCols; ++i)
		{
			r[i] = m_elems[i + NumRows * row];
		}
		return r;
	}

	Rep &Value(const size_t &col, const size_t &row)
	{
		START_PROFILE;
		CRITICAL_ASSERT(col < NumCols);
		CRITICAL_ASSERT(row < NumRows);

		return m_elems[col + row * NumCols];
	}

	const Rep &Value(const size_t &col, const size_t &row) const
	{
		START_PROFILE;
		CRITICAL_ASSERT(col < NumCols);
		CRITICAL_ASSERT(row < NumRows);

		return m_elems[col + row * NumCols];
	}

private:
	Rep m_elems[NumCols * NumRows];
};  // class MatrixBase

template<int NumCols, int NumRows, class Rep>
std::ostream &operator<<(std::ostream &o, const MatrixBase<NumCols, NumRows, Rep> &mat)
{
	for (auto r = 0u; r < NumRows; ++r)
	{
		o << mat.Row(r) << std::endl;
	}
	return o;
}

// Common matrix types
using Mat2x2F = MatrixBase<2, 2, float>;
using Mat3x3F = MatrixBase<3, 3, float>;
using Mat4x4F = MatrixBase<4, 4, float>;
}  // namespace selector

#include "matrix_arithmetic.h"

#endif
