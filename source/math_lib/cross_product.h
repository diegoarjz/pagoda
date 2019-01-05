#ifndef SELECTOR_MATH_LIB_CROSS_PRODUCT_H_
#define SELECTOR_MATH_LIB_CROSS_PRODUCT_H_

namespace selector
{
/**
 * Calculates the cross product of a given vector.
 */
template<class Rep>
inline VecBase<3, Rep> cross_product(const VecBase<3, Rep> &lhs, const VecBase<3, Rep> &rhs)
{
	START_PROFILE;
	return VecBase<3, Rep>(lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2],
	                       lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

}  // namespace selector

#endif
