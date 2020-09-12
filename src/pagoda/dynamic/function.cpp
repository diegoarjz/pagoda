#include "function.h"

#include "null_object_value.h"
#include "type_info.h"
#include "value_visitor.h"

namespace pagoda::dynamic
{
const TypeInfoPtr Function::s_typeInfo = std::make_shared<TypeInfo>("Function");

Function::Function(std::shared_ptr<ICallableBody> callableBody)
  : DynamicValueBase(s_typeInfo), m_callableBody(callableBody), m_callableName("")
{
}

std::shared_ptr<ICallableBody> Function::GetCallableBody() const { return m_callableBody; }

std::string Function::ToString() const { return "Function " + GetCallableName(); }

void Function::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

void Function::SetCallableName(const std::string& name) { m_callableName = name; }

const std::string& Function::GetCallableName() const { return m_callableName; }

void Function::SetArity(const std::size_t& arity) { m_arity = arity; }

const std::size_t& Function::GetArity() const { return m_arity; }

bool Function::IsVariadic() const { return m_isVariadic; }

void Function::SetVariadic(bool variadic) { m_isVariadic = variadic; }

void Function::SetClosure(const std::shared_ptr<DynamicValueTable>& closure) { m_callableBody->SetClosure(closure); }

const std::shared_ptr<DynamicValueTable>& Function::GetClosure() const { return m_callableBody->GetClosure(); }

DynamicValueBasePtr Function::Call(const std::vector<DynamicValueBasePtr>& args)
{
	try {
		m_callableBody->Call(args);
	} catch (DynamicValueBasePtr& returnValue) {
		return returnValue;
	}
	return std::make_shared<NullObject>();
}
}  // namespace pagoda::dynamic
