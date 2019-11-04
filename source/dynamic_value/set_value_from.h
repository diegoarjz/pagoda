#pragma once

#include "binding/native_value_name.h"

#include "../parameter/expression.h"
#include "boolean_value.h"
#include "dynamic_class.h"
#include "dynamic_instance.h"
#include "dynamic_value_base.h"
#include "float_value.h"
#include "function.h"
#include "integer_value.h"
#include "null_object_value.h"
#include "string_value.h"
#include "type_info.h"
#include "value_visitor.h"
#include "vector3.h"

namespace selector
{
/**
 * Exception to be thrown when it is impossible to cast a \c DynamicValueBase to a native type.
 */
template<class T, typename N>
class UnableToAssignFromNative : public std::runtime_error
{
public:
	UnableToAssignFromNative()
	    : std::runtime_error("Unable to assign native value of type " + native_value_name<N>::GetName() +
	                         " to dynamic value of type " + T::s_typeInfo->GetTypeName())
	{
	}
	virtual ~UnableToAssignFromNative() {}
};

/**
 * Tests whether the \c DynamicValueBase of type T can be assigned a native value of type V.
 * In other words, checks for the assignment operator from V.
 */
template<typename T, typename V>
class can_be_assigned
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<C& (C::*)(const V&)>(&C::operator=)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = sizeof(test<T>(0)) == sizeof(yes)
	};
};

template<class T>
class convert_from_native_visitor : public ValueVisitor<void>
{
public:
	convert_from_native_visitor(const T& native) : m_nativeValue(native) {}

	template<typename V>
	typename std::enable_if<can_be_assigned<V, T>::value, void>::type operator()(V& value)
	{
		value = m_nativeValue;
	}

	template<typename V>
	typename std::enable_if<!can_be_assigned<V, T>::value, void>::type operator()(V& value)
	{
		throw UnableToAssignFromNative<V, T>();
	}

private:
	/// The native value to assign.
	const T& m_nativeValue;
};

/**
 * Sets the value of type T stored in the \c DynamicValueBase \p v.
 * If the assignment is impossible, \c UnableToAssignFromNative is thrown.
 * @throws UnableToAssignFromNative
 */
template<typename T>
void set_value_from(DynamicValueBase& v, const typename std::remove_reference<T>::type& n)
{
	convert_from_native_visitor<T> visitor(n);
	apply_visitor(visitor, v);
}
}  // namespace selector
