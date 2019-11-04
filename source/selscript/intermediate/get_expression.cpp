#include "get_expression.h"

#include "ast_visitor.h"

namespace selector
{
namespace ast
{
GetExpression::GetExpression(const ExpressionPtr &lhs, const IdentifierPtr &identifier)
    : m_lhs(lhs), m_identifier(identifier)
{
}

GetExpression::~GetExpression() {}

void GetExpression::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<GetExpression>(shared_from_this()));
}

}  // namespace ast
}  // namespace selector
