#ifndef SELECTOR_PARAMETER_EXPRESSION_H_
#define SELECTOR_PARAMETER_EXPRESSION_H_

#include <memory>
#include <string>
#include <vector>

namespace selector
{
/**
 * Implements expressions that can be evaluated to a parameter.
 */
class Expression
{
public:
	/**
	 * Creates an expression from the \p expressionString.
	 * Parses and creates an intermediary representation that will be executing later on.
	 */
	static std::shared_ptr<Expression> CreateExpression(const std::string& expressionString);

	/**
	 * Returns all variables present in the expression.
	 */
	const std::vector<std::string>& GetVariables() const;

	/**
	 * Evaluates the expression to a float.
	 */
	float GetAsFloat() const;
	/**
	 * Evaluates the expression to a string.
	 */
	std::string GetAsString() const;

private:
	Expression();
	
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
using ExpressionPtr = std::shared_ptr<Expression>;

}  // namespace selector
#endif
