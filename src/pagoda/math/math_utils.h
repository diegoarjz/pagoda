#pragma once

namespace pagoda::math
{
template<class T>
class MathUtils
{
	public:
	static constexpr T pi = T(3.14159265358979323846);
	static constexpr T half_pi = T(3.14159265358979323846) / 2;
	static constexpr T two_pi = T(3.14159265358979323846) * 2;

	static T degrees_to_radians(const T& degrees)
	{
		static const T conv = pi / 180;
		return degrees * conv;
	}

	static T radians_to_degrees(const T& radians)
	{
		static const T conv = 180 / pi;
		return radians * conv;
	}
};

}  // namespace pagoda::math
