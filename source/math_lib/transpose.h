#pragma once

#include "matrix_base.h"

#include <algorithm>

namespace selector
{
/**
 * Modifies the given matrix \p m to be transposed.
 */
template<int NumCols, int NumRows, typename Rep>
MatrixBase<NumCols, NumRows, Rep> &transpose(MatrixBase<NumCols, NumRows, Rep> &m)
{
	for (auto c = 0; c < NumCols; ++c)
	{
		for (auto r = c + 1; r < NumRows; ++r)
		{
			std::swap(m.Value(c, r), m.Value(r, c));
		}
	}
	return m;
}

/**
 * Calculates and returns the transposed matrix.
 * Does not modify the original.
 */
template<int NumCols, int NumRows, typename Rep>
MatrixBase<NumCols, NumRows, Rep> transposed(const MatrixBase<NumCols, NumRows, Rep> &m)
{
	MatrixBase<NumCols, NumRows, Rep> copy(m);
	transpose(copy);
	return copy;
}
}  // namespace selector
