#pragma once

#include "math_utils.h"

namespace pagoda::math
{
template<typename T>
class Radians;

template<class T>
class Degrees
{
	private:
	using mt = MathUtils<T>;

	public:
	explicit Degrees(const T& deg) : m_degrees(deg) {}
	explicit Degrees(const Radians<T>& rad) : m_degrees(mt::radians_to_degrees(static_cast<T>(rad))) {}

	explicit operator T() const { return m_degrees; }

	private:
	T m_degrees;
};
}  // namespace pagoda::math
