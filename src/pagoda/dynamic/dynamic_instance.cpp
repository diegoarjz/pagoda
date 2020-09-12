#include "dynamic_instance.h"

#include "dynamic_class.h"
#include "dynamic_value_table.h"
#include "function.h"
#include "icallable_body.h"
#include "type_info.h"
#include "value_not_found.h"
#include "value_visitor.h"

namespace pagoda::dynamic
{
const TypeInfoPtr DynamicInstance::s_typeInfo = std::make_shared<TypeInfo>("DynamicInstance");

DynamicInstance::DynamicInstance(DynamicClassPtr klass)
  : DynamicValueBase(std::make_shared<TypeInfo>(klass->GetClassName())), ClassBase(klass->GetClassName()),
    m_class(klass)
{
}

DynamicInstance::~DynamicInstance() {}

std::string DynamicInstance::ToString() const { return "<Instance:" + m_class->GetClassName() + ">"; }

void DynamicInstance::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

FunctionPtr DynamicInstance::Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals)
{
	try {
		m_memberTable->Get("this");
	} catch (ValueNotFoundException&) {
		m_memberTable->Declare("this", shared_from_this());
	}
	auto boundMethod = std::make_shared<Function>(callable);
	auto closure = GetInstanceValueTable();
	closure->SetParent(globals);
	boundMethod->SetClosure(closure);
	return boundMethod;
}
}  // namespace pagoda::dynamic
