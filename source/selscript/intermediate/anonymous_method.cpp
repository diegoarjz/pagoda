#include "anonymous_method.h"

#include "ast_visitor.h"

namespace selector
{
namespace ast
{
AnonymousMethod::AnonymousMethod(const ExpressionPtr &instance, const StatementBlockPtr body)
    : m_instance(instance), m_body(body)
{
}
AnonymousMethod::~AnonymousMethod() {}

void AnonymousMethod::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<AnonymousMethod>(shared_from_this()));
}
}  // namespace ast
}  // namespace selector
