#include "dynamic_instance.h"

#include "dynamic_class.h"
#include "dynamic_value_table.h"
#include "function.h"
#include "icallable_body.h"
#include "type_info.h"
#include "value_visitor.h"

namespace selector
{
const TypeInfoPtr DynamicInstance::s_typeInfo = std::make_shared<TypeInfo>("DynamicInstance");

DynamicInstance::DynamicInstance(DynamicClassPtr klass)
    : DynamicValueBase(std::make_shared<TypeInfo>(klass->GetClassName())), m_class(klass)
{
}

DynamicInstance::~DynamicInstance() {}

std::string DynamicInstance::ToString() const { return "<Instance:" + m_class->GetClassName() + ">"; }

void DynamicInstance::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

std::shared_ptr<DynamicValueTable> DynamicInstance::GetInstanceValueTable()
{
	if (!m_valueTable)
	{
		m_valueTable = std::make_shared<DynamicValueTable>(m_class->GetClassName());
		m_valueTable->Declare("this", shared_from_this());
	}

	return m_valueTable;
}

std::shared_ptr<DynamicValueBase> DynamicInstance::GetMember(const std::string& memberName)
{
	return GetInstanceValueTable()->Get(memberName);
}

FunctionPtr DynamicInstance::Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals)
{
	auto boundMethod = std::make_shared<Function>(callable);
	auto closure = GetInstanceValueTable();
	closure->SetParent(globals);
	boundMethod->SetClosure(closure);
	return boundMethod;
}
}  // namespace selector
