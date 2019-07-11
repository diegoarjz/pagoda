#ifndef SELECTOR_PARAMETER_EXPRESSION_H_
#define SELECTOR_PARAMETER_EXPRESSION_H_

#include "parameter.h"

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
	 * Parses and creates an intermediary representation that will be executed later on.
	 */
	static std::shared_ptr<Expression> CreateExpression(const std::string& expressionString);

	Expression();

	/**
	 * Returns all variables present in the expression.
	 */
	const std::vector<std::string>& GetVariables() const;

	/**
	 * Sets the variable with \p variableName name to the value given by \p value.
	 *
	 * If \p value is an \c Expression then it is added as a dependent expression.
	 */
	void SetVariableValue(const std::string& variableName, Parameter value);

	/**
	 * Adds \p e as an \c Expression that is dependent on this \c Expression's value
	 * to be evaluated.
	 */
	void AddDependentExpression(ExpressionPtr e);
	
	/**
	 * Returns the set of dependent expressions.
	 */
	const std::vector<std::weak_ptr<Expression>> GetDependentExpressions() const;

	/**
	 * Marks this \c Expression as dirty, propagating it to all dependent \c Expression.
	 */
	void SetDirty();

	/**
	 * Returns true if the \c Expression is dirty.
	 */
	bool IsDirty() const;

	/**
	 * Evaluates the expression and returns it as a \c Parameter.
	 */
	Parameter GetAsParameter();

	/**
	 * Evaluates the expression to a float.
	 */
	float GetAsFloat();

	/**
	 * Evaluates the expression to a string.
	 */
	std::string GetAsString();

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
using ExpressionPtr = std::shared_ptr<Expression>;

}  // namespace selector
#endif
