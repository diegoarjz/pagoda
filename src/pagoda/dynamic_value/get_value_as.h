#pragma once

#include "binding/can_cast_to_native.h"
#include "binding/native_value_name.h"
#include "type_info.h"

#include "value_visitor.h"

#include <pagoda/common/exception/exception.h>

namespace pagoda
{
/**
 * Exception to be thrown when it is impossible to cast a \c DynamicValueBase to a native type.
 */
template<class T, typename N>
class UnableToCastToNative : public common::exception::Exception
{
public:
	UnableToCastToNative()
	    : common::exception::Exception("Unable to cast dynamic value of type " + T::s_typeInfo->GetTypeName() +
	                                   " to native type " + native_value_name<N>::GetName())
	{
	}
	virtual ~UnableToCastToNative() {}
};

template<class T>
class convert_to_native_visitor : public ValueVisitor<typename std::remove_reference<T>::type>
{
public:
	T operator()(Expression& e)
	{
		auto evaluated = e.Evaluate();
		return apply_visitor(*this, *evaluated);
	}

	template<typename V>
	typename std::enable_if<can_cast_to_native<V, T>::value, T>::type operator()(V& value)
	{
		return static_cast<T>(value);
	}

	template<typename V>
	typename std::enable_if<!can_cast_to_native<V, T>::value, T>::type operator()(V& value)
	{
		throw UnableToCastToNative<V, T>();
	}
};

/**
 * Returns the value of type T stored in the \c DynamicValueBase \p v.
 * If the cast is impossible, \c UnableToCastToNative is thrown.
 * @throws UnableToCastToNative
 */
template<typename T>
typename std::remove_reference<T>::type get_value_as(DynamicValueBase& v)
{
	convert_to_native_visitor<T> visitor;
	return apply_visitor(visitor, v);
}
}  // namespace pagoda
