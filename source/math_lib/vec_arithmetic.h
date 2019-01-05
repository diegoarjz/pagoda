#ifndef SELECTOR_MATH_LIB_VEC_ARITHMETIC_H_
#define SELECTOR_MATH_LIB_VEC_ARITHMETIC_H_

#include <common/profiler.h>

namespace selector
{
/**
 * Sums two vectors.
 */
template<int Size, class Rep>
struct vec_sum
{
	inline static VecBase<Size, Rep> sum(const VecBase<Size, Rep> &lhs, const VecBase<Size, Rep> &rhs)
	{
		VecBase<Size, Rep> res;

		for (auto i = 0u; i < Size; ++i)
		{
			res[i] = lhs[i] + rhs[i];
		}

		return res;
	}
};  // struct vec_sum

template<class Rep>
struct vec_sum<2, Rep>
{
	inline static VecBase<2, Rep> sum(const VecBase<2, Rep> &lhs, const VecBase<2, Rep> &rhs)
	{
		return VecBase<2, Rep>(lhs[0] + rhs[0], lhs[1] + rhs[1]);
	}
};

template<class Rep>
struct vec_sum<3, Rep>
{
	inline static VecBase<3, Rep> sum(const VecBase<3, Rep> &lhs, const VecBase<3, Rep> &rhs)
	{
		return VecBase<3, Rep>(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]);
	}
};

template<class Rep>
struct vec_sum<4, Rep>
{
	inline static VecBase<4, Rep> sum(const VecBase<4, Rep> &lhs, const VecBase<4, Rep> &rhs)
	{
		return VecBase<4, Rep>(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3]);
	}
};

template<int Size, class Rep>
inline VecBase<Size, Rep> operator+(const VecBase<Size, Rep> &lhs, const VecBase<Size, Rep> &rhs)
{
	START_PROFILE;
	return vec_sum<Size, Rep>::sum(lhs, rhs);
}

/**
 * Subtracts two vectors.
 */
template<int Size, class Rep>
struct vec_subtraction
{
	inline static VecBase<Size, Rep> subtract(const VecBase<Size, Rep> &lhs, const VecBase<Size, Rep> &rhs)
	{
		VecBase<Size, Rep> res;

		for (auto i = 0u; i < Size; ++i)
		{
			res[i] = lhs[i] - rhs[i];
		}

		return res;
	}
};  // struct vec_subtract

template<class Rep>
struct vec_subtraction<2, Rep>
{
	inline static VecBase<2, Rep> subtract(const VecBase<2, Rep> &lhs, const VecBase<2, Rep> &rhs)
	{
		return VecBase<2, Rep>(lhs[0] - rhs[0], lhs[1] - rhs[1]);
	}
};

template<class Rep>
struct vec_subtraction<3, Rep>
{
	inline static VecBase<3, Rep> subtract(const VecBase<3, Rep> &lhs, const VecBase<3, Rep> &rhs)
	{
		return VecBase<3, Rep>(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
	}
};

template<class Rep>
struct vec_subtraction<4, Rep>
{
	inline static VecBase<4, Rep> subtract(const VecBase<4, Rep> &lhs, const VecBase<4, Rep> &rhs)
	{
		return VecBase<4, Rep>(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3]);
	}
};

template<int Size, class Rep>
VecBase<Size, Rep> operator-(const VecBase<Size, Rep> &lhs, const VecBase<Size, Rep> &rhs)
{
	START_PROFILE;
	return vec_subtraction<Size, Rep>::subtract(lhs, rhs);
}

/**
 * Scalar Multiplication
 */
template<int Size, class Rep, typename ScalarType>
VecBase<Size, Rep> operator*(const ScalarType &lhs, const VecBase<Size, Rep> &rhs)
{
	START_PROFILE;
	VecBase<Size, Rep> result;
	for (auto i = 0u; i < Size; ++i)
	{
		result[i] = rhs[i] * lhs;
	}
	return result;
}

template<int Size, class Rep, typename ScalarType>
VecBase<Size, Rep> operator*(const VecBase<Size, Rep> &rhs, const ScalarType &lhs)
{
	START_PROFILE;
	VecBase<Size, Rep> result;
	for (auto i = 0u; i < Size; ++i)
	{
		result[i] = rhs[i] * lhs;
	}
	return result;
}

/**
 * Scalar Division
 */
template<int Size, class Rep, typename ScalarType>
VecBase<Size, Rep> operator/(const VecBase<Size, Rep> &rhs, const ScalarType &lhs)
{
	START_PROFILE;
	VecBase<Size, Rep> result;
	for (auto i = 0u; i < Size; ++i)
	{
		result[i] = rhs[i] / lhs;
	}
	return result;
}

/**
 * Scalar Division.
 * Slight abuse of notation.
 */
template<int Size, class Rep, typename ScalarType>
VecBase<Size, Rep> operator/(const ScalarType &lhs, const VecBase<Size, Rep> &rhs)
{
	START_PROFILE;
	VecBase<Size, Rep> result;
	for (auto i = 0u; i < Size; ++i)
	{
		result[i] = lhs / rhs[i];
	}
	return result;
}

}  // namespace selector
#endif
