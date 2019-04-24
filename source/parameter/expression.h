#ifndef SELECTOR_PARAMETER_EXPRESSION_H_
#define SELECTOR_PARAMETER_EXPRESSION_H_

#include <memory>
#include <string>
#include <vector>

namespace selector
{
class Expression
{
public:
	static std::shared_ptr<Expression> CreateExpression(const std::string& expressionString);

	Expression();

	const std::vector<std::string>& GetVariables() const;

	float GetAsFloat() const;
	std::string GetAsString() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
using ExpressionPtr = std::shared_ptr<Expression>;

}  // namespace selector
#endif
