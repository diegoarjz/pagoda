#pragma once

#include "../intermediate/ast.h"
#include "../intermediate/parameter.h"

#include <pagoda/dynamic/icallable_body.h>

#include <memory>

namespace pagoda
{
class ScriptCallableBody : public dynamic::ICallableBody
{
public:
	ScriptCallableBody(ast::IdentifierPtr identifier, ast::StatementBlockPtr body,
	                   const std::vector<ast::ParameterPtr> &parameters);
	virtual ~ScriptCallableBody();

	void Call(const std::vector<dynamic::DynamicValueBasePtr> &args) override;
	void SetClosure(const std::shared_ptr<dynamic::DynamicValueTable> &) override;
	const std::shared_ptr<dynamic::DynamicValueTable> &GetClosure() const override;

private:
	ast::StatementBlockPtr m_body;
	ast::IdentifierPtr m_functionIdentifier;
	std::vector<ast::ParameterPtr> m_parameters;
	std::shared_ptr<dynamic::DynamicValueTable> m_closure;
};
}  // namespace pagoda
