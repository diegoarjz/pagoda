#ifndef SELECTOR_MATH_LIB_LENGHT_H_
#define SELECTOR_MATH_LIB_LENGHT_H_

#include <cmath>

namespace selector
{
/**
 * Calculates the squared length of a vector.
 */
template<int Size, class Rep>
inline Rep squared_length(const VecBase<Size, Rep> &vec)
{
	START_PROFILE;
	Rep len = Rep();

	for (auto i = 0u; i < Size; ++i)
	{
		len += vec[i] * vec[i];
	}

	DBG_ASSERT_MSG(len >= 0, "Length must always be greater or equal to zero");

	return len;
}

/**
 * Calculates the squared length of a \c LineSegment3D.
 */
template<class Rep>
inline Rep squared_length(const LineSegment3D<Rep> &lineSegment)
{
	return squared_length(lineSegment.GetSupportingVector());
}

/**
 * Calculates the length of a vector.
 */
template<int Size, class Rep>
inline Rep length(const VecBase<Size, Rep> &vec)
{
	START_PROFILE;
	DBG_ASSERT_MSG(std::sqrt(squared_length(vec)) >= 0, "Length must always be greater or equal to zero");
	return std::sqrt(squared_length(vec));
}

/**
 * Calculates the length of a \c LineSegment3D.
 */
template<class Rep>
inline Rep length*const LineSegment3D<Rep> &lineSegment)
{
	return length(lineSegment.GetSupportingVector());
}

}  // namespace selector

#endif
