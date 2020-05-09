#include "dynamic_class.h"

#include "callable_body_not_set.h"
#include "dynamic_instance.h"
#include "function.h"
#include "icallable_body.h"
#include "null_object_value.h"
#include "type_info.h"
#include "value_visitor.h"

#include "common/unimplemented.h"

namespace pagoda
{
const TypeInfoPtr DynamicClass::typeInfo = std::make_shared<TypeInfo>("DynamicClass");

DynamicClass::DynamicClass(const std::string& className)
    : DynamicValueBase(typeInfo), ClassBase(className), m_className(className)
{
}

DynamicClass::~DynamicClass() {}

const std::string& DynamicClass::GetClassName() const { return m_className; }

std::string DynamicClass::ToString() const { return "<DynamicClass:" + m_className + ">"; }

void DynamicClass::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

void DynamicClass::SetConstructorBody(std::shared_ptr<ICallableBody> callableBody) { m_callableBody = callableBody; }

/*
 * ICallable overrides
 */
void DynamicClass::SetCallableName(const std::string&) {}

const std::string& DynamicClass::GetCallableName() const { return m_className; }

void DynamicClass::SetArity(const std::size_t& arity) { m_arity = arity; }

const std::size_t& DynamicClass::GetArity() const { return m_arity; }

bool DynamicClass::IsVariadic() const { return m_isVariadic; }

void DynamicClass::SetVariadic(bool variadic) { m_isVariadic = variadic; }

void DynamicClass::SetClosure(const std::shared_ptr<DynamicValueTable>& closure) { m_closure = closure; }

const std::shared_ptr<DynamicValueTable>& DynamicClass::GetClosure() const { return m_closure; }

DynamicValueBasePtr DynamicClass::Call(const std::vector<DynamicValueBasePtr>& args)
{
	if (!m_callableBody)
	{
		throw CallableBodyNotSet(ToString());
	}

	auto instance = std::make_shared<DynamicInstance>(std::dynamic_pointer_cast<DynamicClass>(shared_from_this()));
	try
	{
		// Bind the constructor body to the instance and call the bound constructor
		auto boundConstructor = instance->Bind(m_callableBody);
		boundConstructor->Call(args);
	}
	catch (DynamicValueBasePtr& returnValue)
	{
	}
	return instance;
}

FunctionPtr DynamicClass::Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals)
{
	UNIMPLEMENTED;
}

}  // namespace pagoda
