#pragma once

#include "../get_value_as.h"
#include "can_cast_to_native.h"
#include "type_name.h"

#include <memory>
#include <type_traits>

namespace selector
{
/**
 * Exception thrown if a cast is impossible.
 */
template<class T, class C>
class UnableToCast : public std::runtime_error
{
public:
	UnableToCast() : std::runtime_error("Unable to cast " + type_name<T>::GetName() + " to " + type_name<C>::GetName())
	{
	}
};

/**
 * Casts a \c DynamicValueBase to the type T.
 *
 * If T is a native type, then it uses the \c DynamicValueBase cast operator if it exists.
 * If T is a \c DynamicValueBase returns itself.
 */
template<typename T, typename D>
typename std::enable_if<can_cast_to_native<D, T>::value, T>::type cast_to(std::shared_ptr<D>& dynamicValue)
{
	return get_value_as<T>(*dynamicValue);
}

/**
 * Casts a \c DynamicValueBase to the type T.
 *
 * Used to cast an object of \c DynamicValueBase or any of its subclasses to another \c DynamicValueBase.
 */
template<typename T, typename D>
typename std::enable_if<std::is_base_of<D, T>::value, std::shared_ptr<T>>::type cast_to(
    std::shared_ptr<D>& dynamicValue)
{
	auto ptr = std::dynamic_pointer_cast<T>(dynamicValue);
	if (!ptr)
	{
		throw UnableToCast<D, T>();
	}
	return ptr;
}

/**
 * Casats a \c DynamicValueBase to the type T if both T and D are \c std::shared_ptr.
 */
template<typename T, typename D>
typename std::enable_if<std::is_base_of<typename D::element_type, typename T::element_type>::value, T>::type cast_to(
    D& dynamicValue)
{
	auto ptr = std::dynamic_pointer_cast<typename T::element_type>(dynamicValue);
	if (!ptr)
	{
		throw UnableToCast<typename D::element_type, T>();
	}
	return ptr;
}

}  // namespace selector
