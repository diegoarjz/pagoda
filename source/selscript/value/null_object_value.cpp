#include "null_object_value.h"

#include "value_visitor.h"

namespace sscript
{
const TypeInfoPtr NullObject::typeInfo = std::make_shared<TypeInfo>("Null");

NullObject::NullObject() : BaseValue(typeInfo) {}
NullObject::~NullObject() {}

std::string NullObject::ToString() const { return "null"; }
void NullObject::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
