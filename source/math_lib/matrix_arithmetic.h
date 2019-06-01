#ifndef SELECTOR_MATH_LIB_MATRIX_ARITHMETIC_H_
#define SELECTOR_MATH_LIB_MATRIX_ARITHMETIC_H_

#include <common/profiler.h>

namespace selector
{
template<int NumCols, int NumRows, class Rep>
MatrixBase<NumCols, NumRows, Rep> operator+(const MatrixBase<NumCols, NumRows, Rep> &lhs,
                                            const MatrixBase<NumCols, NumRows, Rep> &rhs)
{
	START_PROFILE;

	MatrixBase<NumCols, NumRows, Rep> result;

	for (auto r = 0u; r < NumRows; ++r)
	{
		for (auto c = 0u; c < NumCols; ++c)
		{
			result.Value(c, r) = lhs.Value(c, r) + rhs.Value(c, r);
		}
	}

	return result;
}

template<int NumCols, int NumRows, class Rep>
MatrixBase<NumCols, NumRows, Rep> operator-(const MatrixBase<NumCols, NumRows, Rep> &lhs,
                                            const MatrixBase<NumCols, NumRows, Rep> &rhs)
{
	START_PROFILE;

	MatrixBase<NumCols, NumRows, Rep> result;

	for (auto r = 0u; r < NumRows; ++r)
	{
		for (auto c = 0u; c < NumCols; ++c)
		{
			result.Value(c, r) = lhs.Value(c, r) - rhs.Value(c, r);
		}
	}

	return result;
}

template<int Size, class Rep>
MatrixBase<Size, Size, Rep> operator*(const MatrixBase<Size, Size, Rep> &lhs, const MatrixBase<Size, Size, Rep> &rhs)
{
	START_PROFILE;

	MatrixBase<Size, Size, Rep> result;

	for (uint32_t c = 0; c < Size; ++c)
	{
		for (uint32_t r = 0; r < Size; ++r)
		{
			for (uint32_t i = 0; i < Size; ++i)
			{
				result.Value(c, r) += lhs.Value(i, r) * rhs.Value(c, i);
			}
		}
	}

	return result;
}

template<int NumCols, int NumRows, class Rep>
MatrixBase<NumCols, NumRows, Rep> operator*(const Rep &lhs, const MatrixBase<NumCols, NumRows, Rep> &rhs)
{
	START_PROFILE;

	MatrixBase<NumCols, NumRows, Rep> result;

	for (auto r = 0u; r < NumRows; ++r)
	{
		for (auto c = 0u; c < NumCols; ++c)
		{
			result.Value(c, r) = lhs * rhs.Value(c, r);
		}
	}

	return result;
}

template<int NumCols, int NumRows, class Rep>
MatrixBase<NumCols, NumRows, Rep> operator*(const MatrixBase<NumCols, NumRows, Rep> &lhs, const Rep &rhs)
{
	START_PROFILE;

	MatrixBase<NumCols, NumRows, Rep> result;

	for (auto r = 0u; r < NumRows; ++r)
	{
		for (auto c = 0u; c < NumCols; ++c)
		{
			result.Value(c, r) = lhs.Value(c, r) * rhs;
		}
	}

	return result;
}

template<int Size, class Rep>
VecBase<Size, Rep> operator*(const VecBase<Size, Rep> &v, const MatrixBase<Size, Size, Rep> &m)
{
	START_PROFILE;

	VecBase<Size, Rep> result;

	for (auto c = 0u; c < Size; ++c)
	{
		for (auto r = 0u; r < Size; ++r)
		{
			result[c] += v[c] * m.Value(c, r);
		}
	}

	return result;
}

template<int Size, class Rep>
VecBase<Size, Rep> operator*(const MatrixBase<Size, Size, Rep> &m, const VecBase<Size, Rep> &v)
{
	START_PROFILE;

	VecBase<Size, Rep> result;

	for (auto r = 0u; r < Size; ++r)
	{
		for (auto c = 0u; c < Size; ++c)
		{
			result[r] += m.Value(c, r) * v[r];
		}
	}

	return result;
}

}  // namespace selector
#endif
