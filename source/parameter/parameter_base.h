#pragma once
#ifndef SELECTOR_PARAMETER_PARAMETER_BASE_H_
#define SELECTOR_PARAMETER_PARAMETER_BASE_H_

#include "common/profiler.h"

#include "expression.h"
#include "parameter_identifier.h"

namespace selector
{
/**
 * The available parameter types.
 */
enum class ParameterTypes
{
	Float,
	String,
	Enumeration
};  // enum class ParameterTypes

/**
 * Base class for a Parameter.
 *
 * Templated Parameter classes derive from this class.
 */
class ParameterBase
{
public:
	explicit ParameterBase(const ParameterIdentifier& name);
	virtual ~ParameterBase() {}

	const std::string& Name() const;
	virtual ParameterTypes Type() const = 0;

	void SetExpression(std::shared_ptr<Expression>& e);
	std::shared_ptr<Expression> GetExpression() const;

	template<class T>
	typename T::value_t Set(const typename T::value_t& value)
	{
		typename T::parameter_t* derived = dynamic_cast<typename T::parameter_t*>(this);
		return derived->SetValue(value);
	}

	template<class T>
	const typename T::value_t& Get() const
	{
		const typename T::parameter_t* derived = dynamic_cast<const typename T::parameter_t*>(this);
		return derived->GetValue();
	}

	template<class T>
	typename T::value_t& Get()
	{
		typename T::parameter_t* derived = dynamic_cast<typename T::parameter_t*>(this);
		return derived->GetValue();
	}

private:
	ParameterIdentifier m_parameterName;
	std::shared_ptr<Expression> m_expression;
};  // class ParameterBase

}  // namespace selector

#endif
