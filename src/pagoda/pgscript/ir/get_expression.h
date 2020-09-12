#pragma once

#include "ast.h"

namespace pagoda
{
namespace ast
{
class GetExpression : public Expression
{
	public:
	GetExpression() = default;
	GetExpression(const ExpressionPtr &lhs, const IdentifierPtr &identifier);
	virtual ~GetExpression();

	void AcceptVisitor(AstVisitor *v) override;

	const ExpressionPtr &GetLhs() const { return m_lhs; }
	const IdentifierPtr GetIdentifier() const { return m_identifier; }

	private:
	ExpressionPtr m_lhs;
	IdentifierPtr m_identifier;
};
using GetExpressionPtr = std::shared_ptr<GetExpression>;
}  // namespace ast
}  // namespace pagoda
