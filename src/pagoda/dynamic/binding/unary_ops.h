#pragma once

#include "../dynamic_value_base.h"
#include "../value_visitor.h"
#include "has_operators.h"

#include <pagoda/common/exception/exception.h>

namespace pagoda::dynamic
{
/**
 * Exception thrown when the negate operator is undefined for a \c DynamicValueBase.
 */
template<class T>
class UndefinedNegateOperator : public common::exception::Exception
{
	public:
	UndefinedNegateOperator()
	  : common::exception::Exception("Undefined negate operator for " + T::s_typeInfo->GetTypeName() + " type")
	{
	}
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
class UndefinedMinusOperator : public common::exception::Exception
{
	public:
	UndefinedMinusOperator()
	  : common::exception::Exception("Undefined minus operator for " + T::s_typeInfo->GetTypeName() + " type")
	{
	}
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
}  // namespace pagoda::dynamic
