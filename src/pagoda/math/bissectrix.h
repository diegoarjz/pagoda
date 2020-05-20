#pragma once

#include "orthogonal.h"

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace pagoda::math
{
template<class Rep, int Size>
boost::qvm::vec<Rep, Size> bissectrix(const boost::qvm::vec<Rep, Size> &v1, const boost::qvm::vec<Rep, Size> &v2)
{
	if (boost::qvm::cross(v1, v2) == boost::qvm::vec<Rep, Size>{0, 0, 0})
	{
		return boost::qvm::normalized(orthogonal(v1));
	}
	return boost::qvm::normalized(Rep(0.5) * (v1 + v2));
}
}  // namespace pagoda::math
