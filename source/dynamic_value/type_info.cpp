#include "type_info.h"

#include "dynamic_value_base.h"
#include "value_visitor.h"

namespace selector
{
const std::shared_ptr<TypeInfo> TypeInfo::s_typeInfo = std::make_shared<TypeInfo>("TypeInfo");

TypeInfo::TypeInfo() : DynamicValueBase(s_typeInfo), m_typeName{} {}

TypeInfo::TypeInfo(const std::string& name) : DynamicValueBase(s_typeInfo), m_typeName(name) {}

std::string TypeInfo::GetTypeName() const { return m_typeName; }

std::string TypeInfo::ToString() const { return "<TypeInfo:" + m_typeName + ">"; }

void TypeInfo::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace selector
