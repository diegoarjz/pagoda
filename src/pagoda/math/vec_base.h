#ifndef PAGODA_MATH_LIB_VEC_BASE_H
#define PAGODA_MATH_LIB_VEC_BASE_H

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <cstring>

#include <boost/qvm/vec.hpp>

namespace pagoda::math
{
template<int Size, class Rep>
std::ostream& operator<<(std::ostream& o, const boost::qvm::vec<Rep, Size>& vec)
{
	o << "[";
	for (auto i = 0u; i < Size; ++i)
	{
		o << vec.a[i] << ",";
	}
	o << "]";
	return o;
}

template<int Size, class Rep>
std::string to_string(const boost::qvm::vec<Rep, Size>& vec)
{
	std::stringstream ss;
	ss << vec;
	return ss.str();
}

// Common vector types
using Vec2F = boost::qvm::vec<float, 2>;
using Vec3F = boost::qvm::vec<float, 3>;
using Vec4F = boost::qvm::vec<float, 4>;

using Vec2D = boost::qvm::vec<double, 2>;
using Vec3D = boost::qvm::vec<double, 3>;
using Vec4D = boost::qvm::vec<double, 4>;

}  // namespace pagoda::math

#endif
