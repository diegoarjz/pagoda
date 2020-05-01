#include "set_expression.h"

#include "ast_visitor.h"

namespace pagoda
{
namespace ast
{
SetExpression::SetExpression(const ExpressionPtr &lhs, const IdentifierPtr &identifier, const ExpressionPtr &rhs)
    : m_lhs(lhs), m_identifier(identifier), m_rhs(rhs)
{
}
SetExpression::~SetExpression() {}

void SetExpression::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<SetExpression>(shared_from_this()));
}
}  // namespace ast
}  // namespace pagoda
