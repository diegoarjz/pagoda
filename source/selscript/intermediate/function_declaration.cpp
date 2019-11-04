#include "function_declaration.h"

#include "ast_visitor.h"

namespace selector
{
namespace ast
{
FunctionDeclaration::~FunctionDeclaration() {}
void FunctionDeclaration::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<FunctionDeclaration>(shared_from_this()));
}
}  // namespace ast
}  // namespace selector
