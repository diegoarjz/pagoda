#pragma once

#include <boost/qvm/mat.hpp>

#include <ostream>

namespace pagoda
{
template<int NumCols, int NumRows, class Rep>
std::ostream &operator<<(std::ostream &o, const boost::qvm::mat<Rep, NumRows, NumCols> &mat)
{
	for (auto r = 0u; r < NumRows; ++r)
	{
		o << "[";
		for (auto c = 0u; c < NumCols; ++c)
		{
			o << mat.a[r][c] << " ";
		}
		o << "]" << std::endl;
	}
	return o;
}

// Common matrix types
using Mat2x2F = boost::qvm::mat<float, 2, 2>;
using Mat3x3F = boost::qvm::mat<float, 3, 3>;
using Mat4x4F = boost::qvm::mat<float, 4, 4>;

}  // namespace pagoda
