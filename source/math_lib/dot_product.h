#ifndef SELECTOR_MATH_LIB_DOT_PRODUCT_H_
#define SELECTOR_MATH_LIB_DOT_PRODUCT_H_

namespace selector
{
/**
 * Calculates the dot product between two vectors.
 */
template<int Size, class Rep>
Rep dot_product(const VecBase<Size, Rep> &v1, const VecBase<Size, Rep> &v2)
{
	Rep result(0);

	for (auto i = 0u; i < Size; ++i)
	{
		result += v1[i] * v2[i];
	}

	return result;
}
}  // namespace selector

#endif
