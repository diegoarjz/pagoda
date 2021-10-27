#include "parameter.h"

#include <pagoda/pgscript/interpreter/interpreter.h>
#include <pagoda/pgscript/parser/parser.h>

namespace pagoda::objects
{
struct ParameterBase::Expression
{
	std::string m_expression;
	ast::ProgramPtr m_compiledExpression;
};

ParameterBase::ParameterBase(const std::string& name)
  : m_name{name}, m_label{name}
{
}

ParameterBase::ParameterBase(const std::string& name, const std::string& label)
  : m_name{name}, m_label{label}
{
}

ParameterBase::~ParameterBase()
{
}

bool ParameterBase::HasFlag(Flag flag) const
{
	return m_flags.test(static_cast<size_t>(flag));
}

void ParameterBase::SetFlag(Flag flag, bool val)
{
	m_flags.set(static_cast<size_t>(flag), val);
}

////////////////////////////////////////
/// \name Getters and Setters
////////////////////////////////////////
const std::string& ParameterBase::GetName() const
{
	return m_name;
}
const std::string& ParameterBase::GetLabel() const
{
	return m_label;
}

////////////////////////////////////////
/// \name Expressions
////////////////////////////////////////
bool ParameterBase::HasExpression() const
{
	return m_expression != nullptr;
}

void ParameterBase::SetExpression(const std::string& e)
{
	if (m_expression == nullptr) {
		m_expression = std::make_unique<Expression>();
	}
	m_expression->m_expression = e;
}

const std::string& ParameterBase::GetExpression() const
{
	static const std::string& sEmptyExpression{""};
	if (m_expression == nullptr) {
		return sEmptyExpression;
	}
	return m_expression->m_expression;
}

void ParameterBase::EvaluateExpression()
{
	if (m_expression == nullptr) {
		return;
	}

	if (m_expression->m_compiledExpression == nullptr) {
		m_expression->m_compiledExpression =
		  Parser().Parse(m_expression->m_expression);
	}

	if (m_expression->m_compiledExpression == nullptr) {
		return;
	}

	Interpreter interpreter;
	interpreter.Interpret(m_expression->m_compiledExpression);
	if (auto value = interpreter.GetLastEvaluatedExpression()) {
		FromDynamicValue(value);
	}
}

////////////////////////////////////////
/// \name Callbacks
////////////////////////////////////////
void ParameterBase::OnChanged(ParameterBase::OnChangedCallback cb)
{
	m_onChangedCallbacks.push_back(cb);
}

void ParameterBase::changed()
{
	for (const auto& cb : m_onChangedCallbacks) {
		cb(shared_from_this());
	}
}

////////////////////////////////////////
/// \name Conversion and Serialization
////////////////////////////////////////

template<>
dynamic::DynamicValueBasePtr Parameter<std::string>::ToDynamicValue()
{
	return std::make_shared<dynamic::String>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<float>::ToDynamicValue()
{
	return std::make_shared<dynamic::FloatValue>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<int>::ToDynamicValue()
{
	return std::make_shared<dynamic::Integer>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<bool>::ToDynamicValue()
{
	return std::make_shared<dynamic::Boolean>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<math::Plane<float>>::ToDynamicValue()
{
	return std::make_shared<dynamic::DynamicPlane>(*m_valuePtr);
}

}  // namespace pagoda::objects
