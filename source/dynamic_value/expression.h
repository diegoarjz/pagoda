#ifndef SELECTOR_PARAMETER_EXPRESSION_H_
#define SELECTOR_PARAMETER_EXPRESSION_H_

#include "../parameter/variable.h"

#include "dynamic_value_base.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace selector
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

/**
 * Implements expressions that can be evaluated to a parameter.
 */
class Expression : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Creates an expression from the \p expressionString.
	 * Parses and creates an intermediary representation that will be executed later on.
	 */
	static std::shared_ptr<Expression> CreateExpression(const std::string& expressionString);

	Expression();

	/**
	 * Returns all variables present in the expression.
	 */
	const std::unordered_set<Variable, Variable::Hash>& GetVariables() const;

	/**
	 * Sets the variable \p variable to the value given by \p value.
	 *
	 * If \p value is an \c Expression then it is added as a dependent expression.
	 */
	void SetVariableValue(const Variable& variable, DynamicValueBasePtr value);

	/**
	 * Sets the variable whith \p variableName to the value given by \p value.
	 */
	void SetVariableValue(const std::string& variableName, DynamicValueBasePtr value);

	/**
	 * Adds \p e as an \c Expression that is dependent on this \c Expression's value
	 * to be evaluated.
	 */
	void AddDependentExpression(std::shared_ptr<Expression> e);

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

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;

    DynamicValueBasePtr Evaluate();

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
using ExpressionPtr = std::shared_ptr<Expression>;

}  // namespace selector
#endif
