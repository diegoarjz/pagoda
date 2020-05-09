#include "null_object_value.h"

#include "type_info.h"
#include "value_visitor.h"

namespace pagoda
{
const TypeInfoPtr NullObject::s_typeInfo = std::make_shared<TypeInfo>("Null");

NullObject::NullObject() : DynamicValueBase(s_typeInfo) {}
NullObject::~NullObject() {}

std::string NullObject::ToString() const { return "null"; }

void NullObject::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace pagoda
