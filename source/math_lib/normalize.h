#ifndef SELETOR_MATH_LIB_NORMALIZE_H_
#define SELETOR_MATH_LIB_NORMALIZE_H_

#include <iomanip>

namespace selector
{
/**
 * Calculates and returns the normalized vector.
 */
template<int Size, class Rep>
inline VecBase<Size, Rep> normalized(const VecBase<Size, Rep>& vec)
{
	START_PROFILE;
	DBG_ASSERT_MSG(length(vec) > 0, "Lenght of a vector must not be zero to normalize.");
	return vec / length(vec);
}

/**
 * Modifies the given vector to be normalized.
 */
template<int Size, class Rep>
VecBase<Size, Rep>& normalize(VecBase<Size, Rep>& vec)
{
	START_PROFILE;
	DBG_ASSERT_MSG(length(vec) > 0, "Lenght of a vector must not be zero to normalize.");

	// TODO: What to do with rounding errors?
	auto inv_len = 1 / length(vec);

	for (auto i = 0u; i < Size; ++i)
	{
		vec[i] *= inv_len;
	}

	return vec;
}

}  // namespace selector

#endif
