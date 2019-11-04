#pragma once

#include "ast.h"

namespace selector
{
namespace ast
{
class SetExpression : public Expression
{
public:
	SetExpression() = default;
	SetExpression(const ExpressionPtr &lhs, const IdentifierPtr &identifier, const ExpressionPtr &rhs);
	virtual ~SetExpression();

	void AcceptVisitor(AstVisitor *v) override;

	const ExpressionPtr &GetLhs() const { return m_lhs; }
	const IdentifierPtr GetIdentifier() const { return m_identifier; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }

private:
	ExpressionPtr m_lhs;
	IdentifierPtr m_identifier;
	ExpressionPtr m_rhs;
};
using SetExpressionPtr = std::shared_ptr<SetExpression>;
}  // namespace ast
}  // namespace selector
