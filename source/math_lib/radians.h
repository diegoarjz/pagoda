#pragma once

#include "math_utils.h"

namespace pagoda
{
template<class T>
class Degrees;

template<class T>
class Radians
{
private:
	using mt = MathUtils<T>;

public:
	explicit Radians(const T& rad) : m_radians(rad) {}
	explicit Radians(const Degrees<T>& deg) : m_radians(mt::degrees_to_radians(static_cast<T>(deg))) {}

	explicit operator T() const { return m_radians; }

private:
	T m_radians;
};
}  // namespace pagoda

