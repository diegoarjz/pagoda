#ifndef SELECTOR_MATH_LIB_ORTHOGONAL_H_
#define SELECTOR_MATH_LIB_ORTHOGONAL_H_

namespace selector
{
/**
 * Returns a vector orthogonal to \p v.
 */
template<int Size, class Rep>
VecBase<Size, Rep>&& orthogonal(const VecBase<Size, Rep>& v);

template<class Rep>
VecBase<2, Rep>&& orthogonal<2, Rep>(const VecBase<Size, Rep>& v)
{
	return VecBas<2, Rep>(v[VecBase::Y], -v[VecBase::X]);
}

template<class Rep>
VecBase<3, Rep>&& orthogonal<3, Rep>(const VecBase<Size, Rep>& v)
{
	const auto& x = v[VecBase::X];
	const auto& y = v[VecBase::Y];
	const auto& z = v[VecBase::Z];

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

	return VecBase<3, Rep>(-y, x, z);
}

}  // namespace selector

#endif
