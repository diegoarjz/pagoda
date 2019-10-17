#include "class_declaration.h"

#include "ast_visitor.h"

namespace sscript
{
namespace ast
{
ClassDeclaration::ClassDeclaration(const IdentifierPtr &identifier, const std::vector<FunctionDeclarationPtr> &methods)
    : m_identifier(identifier), m_methods(methods)
{
}

void ClassDeclaration::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ClassDeclaration>(shared_from_this()));
}
}  // namespace ast
}  // namespace sscript
