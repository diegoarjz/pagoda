#include "type_info.h"

#include "boolean_value.h"
#include "value_visitor.h"

#include <iostream>

namespace sscript
{
const std::shared_ptr<TypeInfo> TypeInfo::typeInfo = std::make_shared<TypeInfo>("TypeInfo");

TypeInfo::TypeInfo(const std::string& name) : BaseValue(typeInfo), m_typeName(name) {}

std::string TypeInfo::ToString() const { return "<TypeInfo:" + m_typeName + ">"; }
void TypeInfo::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

std::shared_ptr<Boolean> TypeInfo::operator==(const TypeInfo& o) const { return std::make_shared<Boolean>(this == &o); }
std::shared_ptr<Boolean> TypeInfo::operator!=(const TypeInfo& o) const { return std::make_shared<Boolean>(this != &o); }
}  // namespace sscript
