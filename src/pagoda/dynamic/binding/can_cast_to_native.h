#pragma once

namespace pagoda::dynamic
{
/**
 * Tests whether the \c DynamicValueBase of type T can be cast to a native value of type V.
 * In other words, checks for the cast operator to V.
 */
template<typename T, typename V>
class can_cast_to_native
{
	using yes = char;
	struct no
	{
		float x[2];
	};

	template<typename C>
	static yes test(decltype(&C::operator V));

	template<typename C>
	static no test(...);

	public:
	enum
	{
		value = sizeof(test<T>(0)) == sizeof(yes)
	};
};
}  // namespace pagoda::dynamic
