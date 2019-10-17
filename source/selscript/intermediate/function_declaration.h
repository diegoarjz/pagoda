#pragma once

#include "ast.h"

namespace sscript
{
namespace ast
{
class Parameter;
using ParameterPtr = std::shared_ptr<Parameter>;

struct FunctionDeclaration : public Statement
{
	FunctionDeclaration(IdentifierPtr funIdentifier, const std::vector<ParameterPtr> &params, StatementBlockPtr body)
	    : m_functionIdentifier(funIdentifier), m_parameters(params), m_functionBody(body)
	{
	}
	virtual ~FunctionDeclaration();

	IdentifierPtr GetIdentifier() const { return m_functionIdentifier; }
	const std::vector<ParameterPtr> &GetParameters() const { return m_parameters; }
	StatementBlockPtr GetFunctionBody() const { return m_functionBody; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	IdentifierPtr m_functionIdentifier;
	std::vector<ParameterPtr> m_parameters;
	StatementBlockPtr m_functionBody;
};
using FunctionDeclarationPtr = std::shared_ptr<FunctionDeclaration>;

}  // namespace ast
}  // namespace sscript
