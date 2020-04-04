#ifndef SELECTOR_MATH_LIB_ORTHOGONAL_H_
#define SELECTOR_MATH_LIB_ORTHOGONAL_H_

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_access.hpp>

namespace selector
{
/**
 * Returns a vector orthogonal to \p v.
 */
template<class Rep>
boost::qvm::vec<Rep, 2> orthogonal(const boost::qvm::vec<Rep, 2>& v)
{
	return boost::qvm::vec<Rep, 2>{Y(v), -X(v)};
}

template<class Rep>
boost::qvm::vec<Rep, 3> orthogonal(const boost::qvm::vec<Rep, 3>& v)
{
	const auto& x = X(v);
	const auto& y = Y(v);
	const auto& z = Z(v);

	if (x == Rep(0))
	{
		return boost::qvm::vec<Rep, 3>{Rep(1), Rep(0), Rep(0)};
	}

	if (y == Rep(0))
	{
		return boost::qvm::vec<Rep, 3>{Rep(0), Rep(1), Rep(0)};
	}

	if (z == Rep(0))
	{
		return boost::qvm::vec<Rep, 3>{Rep(0), Rep(0), Rep(1)};
	}

	return boost::qvm::vec<Rep, 3>{1, 1, -(x + y) / z};
}

}  // namespace selector

#endif
