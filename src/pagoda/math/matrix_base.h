#pragma once

#include <boost/qvm/mat.hpp>

#include <ostream>

namespace pagoda::math
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
// using boost::qvm::mat<float, 2, 2> = boost::qvm::mat<float, 2, 2>;
// using boost::qvm::mat<float, 3, 3> = boost::qvm::mat<float, 3, 3>;
// using boost::qvm::mat<float, 4, 4> = boost::qvm::mat<float, 4, 4>;

}  // namespace pagoda::math
