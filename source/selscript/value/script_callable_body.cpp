#include "script_callable_body.h"

namespace selector
{
ScriptCallableBody::ScriptCallableBody(ast::IdentifierPtr identifier, ast::StatementBlockPtr body,
                                       const std::vector<ast::ParameterPtr> &parameters)
    : m_body(body), m_functionIdentifier(identifier), m_parameters(parameters)
{
}
ScriptCallableBody::~ScriptCallableBody() {}

void ScriptCallableBody::Call(const std::vector<DynamicValueBasePtr> &args) {}

void ScriptCallableBody::SetClosure(const std::shared_ptr<DynamicValueTable> &closure) { m_closure = closure; }

const std::shared_ptr<DynamicValueTable> &ScriptCallableBody::GetClosure() const { return m_closure; }

}  // namespace selector
