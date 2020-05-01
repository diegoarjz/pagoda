#include "parameter.h"

#include "ast_visitor.h"

namespace pagoda
{
namespace ast
{
Parameter::Parameter(const std::string& identifier) : m_identifierName(identifier) {}
void Parameter::AcceptVisitor(AstVisitor* v) { v->Visit(std::dynamic_pointer_cast<Parameter>(shared_from_this())); }
}  // namespace ast
}  // namespace pagoda
