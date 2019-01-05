#ifndef SELECTOR_PARAMETER_EXPRESSION_H_
#define SELECTOR_PARAMETER_EXPRESSION_H_

#include <regex>
#include <string>
#include <unordered_map>

namespace selector
{
class ParameterBase;

class ExpressionVariable
{
public:
	ExpressionVariable(const std::string& variable, const std::string& original_variable);

	const std::string& GetOriginalVariable() const { return m_originalVariable; }
	const std::string& GetVariableName() const { return m_variable; }
	void SetReferencedParameter(std::shared_ptr<ParameterBase>& parameter) { m_referencedParameter = parameter; }
	std::shared_ptr<ParameterBase> GetReferencedParameter() const { return m_referencedParameter; }

private:
	std::string m_variable;
	std::string m_originalVariable;
	std::shared_ptr<ParameterBase> m_referencedParameter;
};  // class ExpressionVariable

class Expression
{
public:
	using VariableContainer_t = std::unordered_map<std::string, std::shared_ptr<ExpressionVariable>>;

	class ExpressionBackend
	{
	public:
		virtual void Evaluate() = 0;
	};  // class ExpressionBackend

	explicit Expression(const std::string& expression);

	const std::string& GetExpression() const { return m_expression; }
	void SetCompiledExpression(const std::string& expression) { m_compiledExpression = expression; }
	const std::string& GetCompiledExpression() const { return m_compiledExpression; }

	const VariableContainer_t& GetVariables() const { return m_variables; }
	void SetVariables(const VariableContainer_t& vars) { m_variables = vars; }

	void SetExpressionBackend(const std::shared_ptr<ExpressionBackend>& backend) { m_expressionBackend = backend; }
	void Evaluate() { m_expressionBackend->Evaluate(); }

private:
	std::string m_expression;
	// available variables
	VariableContainer_t m_variables;
	// compiled expression
	std::string m_compiledExpression;
	std::shared_ptr<ExpressionBackend> m_expressionBackend;
};  // class Expression

}  // namespace selector
#endif
