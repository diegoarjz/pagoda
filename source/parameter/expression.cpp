#include "expression.h"

#include "common/profiler.h"
#include "parameter_validation.h"

namespace selector
{
ExpressionVariable::ExpressionVariable(const std::string &variable, const std::string &original_variable)
    : m_variable(variable), m_originalVariable(original_variable)
{
}

Expression::Expression(const std::string &expression) : m_expression(expression), m_expressionBackend(nullptr) {}

}  // namespace selector
