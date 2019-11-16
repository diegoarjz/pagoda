#pragma once

#include "common/exception.h"

#include "../dynamic_value_base.h"
#include "../value_visitor.h"
#include "has_operators.h"

namespace selector
{
/**
 * Exception thrown when the negate operator is undefined for a \c DynamicValueBase.
 */
template<class T>
class UndefinedNegateOperator : public Exception
{
public:
	UndefinedNegateOperator() : Exception("Undefined negate operator for " + T::s_typeInfo->GetTypeName() + " type") {}
};

/**
 * Applies the negate operator to an object of \c DynamicValueBase if its negate operator is defined.
 */
struct negate
{
	template<typename T>
	static typename std::enable_if<has_negate_operator<T>::value, DynamicValueBasePtr>::type apply(const T &v)
	{
		return std::make_shared<decltype(!v)>(!v);
	}

	template<typename T>
	static typename std::enable_if<!has_negate_operator<T>::value, DynamicValueBasePtr>::type apply(const T &v)
	{
		throw UndefinedNegateOperator<T>();
	}
};

/**
 * Exception thrown when the minus operator is undefined for a \c DynamicValueBase.
 */
template<class T>
class UndefinedMinusOperator : public Exception
{
public:
	UndefinedMinusOperator() : Exception("Undefined minus operator for " + T::s_typeInfo->GetTypeName() + " type") {}
};

/**
 * Applies the minus operator to an object of \c DynamicValueBase if its minus operator is defined.
 */
struct minus
{
	template<typename T>
	static typename std::enable_if<has_minus_operator<T>::value, DynamicValueBasePtr>::type apply(const T &v)
	{
		return std::make_shared<decltype(-v)>(-v);
	}

	template<typename T>
	static typename std::enable_if<!has_minus_operator<T>::value, DynamicValueBasePtr>::type apply(const T &v)
	{
		throw UndefinedMinusOperator<T>();
	}
};

/**
 * Applies an unary operation to a \c DynamicValueBase.
 */
template<class Op>
struct unary_ops_dispatcher : public ValueVisitor<DynamicValueBasePtr>
{
	template<typename Operand>
	DynamicValueBasePtr operator()(const Operand &operand)
	{
		return Op::apply(operand);
	}
};
}  // namespace selector
