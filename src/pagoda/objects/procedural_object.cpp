#include "procedural_object.h"

#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_visitor.h>

using namespace pagoda::dynamic;

namespace pagoda::objects
{
const TypeInfoPtr ProceduralObject::s_typeInfo = std::make_shared<TypeInfo>("ProceduralObject");

ProceduralObject::ProceduralObject() : BuiltinClass(s_typeInfo) {}

ProceduralObject::~ProceduralObject() {}

std::string ProceduralObject::ToString() const { return "<ProceduralObject>"; }

void ProceduralObject::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace pagoda::objects
