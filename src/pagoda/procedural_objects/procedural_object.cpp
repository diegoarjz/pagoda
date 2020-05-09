#include "procedural_object.h"

#include "dynamic_value/type_info.h"

namespace pagoda
{
const TypeInfoPtr ProceduralObject::s_typeInfo = std::make_shared<TypeInfo>("ProceduralObject");

ProceduralObject::ProceduralObject() : BuiltinClass(s_typeInfo) {}

ProceduralObject::~ProceduralObject() {}

std::string ProceduralObject::ToString() const { return "<ProceduralObject>"; }

void ProceduralObject::AcceptVisitor(ValueVisitorBase& visitor) { throw Exception("Unimplemented"); }
}  // namespace pagoda
