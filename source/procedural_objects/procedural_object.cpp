#include "procedural_object.h"

namespace selector
{
const TypeInfoPtr ProceduralObject::s_typeInfo = std::make_shared<TypeInfo>("ProceduralObject");

ProceduralObject::ProceduralObject() : BuiltinClass(s_typeInfo) {}

ProceduralObject::~ProceduralObject() {}

std::string ProceduralObject::ToString() const { return "<ProceduralObject>"; }

void ProceduralObject::AcceptVisitor(ValueVisitorBase& visitor) { throw std::runtime_error("Unimplemented"); }
}  // namespace selector
