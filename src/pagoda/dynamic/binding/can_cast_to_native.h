#pragma once

#include <type_traits>

namespace pagoda::dynamic
{
template<typename T, typename U, typename = void>
struct is_safely_castable : std::false_type
{
};

template<typename T, typename U>
struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type
{
};

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
		char x[2];
	};

	template<typename C>
	static yes& test(decltype(&C::operator V));

	template<typename C>
	static no& test(...);

	public:
	enum
	{
		value = is_safely_castable<T, V>::value
	};
};

}  // namespace pagoda::dynamic
