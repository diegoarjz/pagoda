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
}

}  // namespace selector
#endif
