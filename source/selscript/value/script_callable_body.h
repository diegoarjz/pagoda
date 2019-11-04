#pragma once

#include "../../dynamic_value/icallable_body.h"
#include "../intermediate/ast.h"
#include "../intermediate/parameter.h"

namespace selector
{
class ScriptCallableBody : public ICallableBody
{
public:
	ScriptCallableBody(ast::IdentifierPtr identifier, ast::StatementBlockPtr body,
	                   const std::vector<ast::ParameterPtr> &parameters);
	virtual ~ScriptCallableBody();

	void Call(const std::vector<DynamicValueBasePtr> &args) override;
	void SetClosure(const std::shared_ptr<DynamicValueTable> &) override;
	const std::shared_ptr<DynamicValueTable> &GetClosure() const override;

private:
	ast::StatementBlockPtr m_body;
	ast::IdentifierPtr m_functionIdentifier;
	std::vector<ast::ParameterPtr> m_parameters;
	std::shared_ptr<DynamicValueTable> m_closure;
};
}  // namespace selector
