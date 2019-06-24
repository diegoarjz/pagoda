#ifndef SELECTOR_MATH_LIB_ORTHOGONAL_H_
#define SELECTOR_MATH_LIB_ORTHOGONAL_H_

#include "vec_base.h"

namespace selector
{
/**
 * Returns a vector orthogonal to \p v.
 */
template<class Rep>
VecBase<2, Rep> orthogonal(const VecBase<2, Rep>& v)
{
	return VecBase<2, Rep>(v[1], -v[0]);
}

template<class Rep>
VecBase<3, Rep> orthogonal(const VecBase<3, Rep>& v)
{
	const auto& x = v[0];
	const auto& y = v[1];
	const auto& z = v[2];

	if (x == Rep(0))
	{
		return VecBase<3, Rep>(Rep(1), Rep(0), Rep(0));
	}

	if (y == Rep(0))
	{
		return VecBase<3, Rep>(Rep(0), Rep(1), Rep(0));
	}

	if (z == Rep(0))
	{
		return VecBase<3, Rep>(Rep(0), Rep(0), Rep(1));
	}

	// return VecBase<3, Rep>(-y, x, z);
	return VecBase<3, Rep>(1, 1, -(x + y) / z);
}

}  // namespace selector

#endif
