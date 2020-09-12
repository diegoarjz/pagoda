#pragma once

#include "ast.h"
#include "function_declaration.h"

namespace pagoda
{
namespace ast
{
class ClassDeclaration : public Statement
{
	public:
	ClassDeclaration() = default;
	ClassDeclaration(const IdentifierPtr &identifier, const std::vector<FunctionDeclarationPtr> &methods);
	virtual ~ClassDeclaration(){};

	void AcceptVisitor(AstVisitor *v) override;

	const IdentifierPtr &GetIdentifier() const { return m_identifier; }
	const std::vector<FunctionDeclarationPtr> &GetMethods() const { return m_methods; }

	private:
	IdentifierPtr m_identifier;
	std::vector<FunctionDeclarationPtr> m_methods;
};
using ClassDeclarationPtr = std::shared_ptr<ClassDeclaration>;

}  // namespace ast
}  // namespace pagoda
