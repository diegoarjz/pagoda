#include "procedural_object.h"

#include <pagoda/dynamic/type_info.h>

using namespace pagoda::dynamic;

namespace pagoda::objects
{
const TypeInfoPtr ProceduralObject::s_typeInfo = std::make_shared<TypeInfo>("ProceduralObject");

ProceduralObject::ProceduralObject() : BuiltinClass(s_typeInfo) {}

ProceduralObject::~ProceduralObject() {}

std::string ProceduralObject::ToString() const { return "<ProceduralObject>"; }

void ProceduralObject::AcceptVisitor(ValueVisitorBase& visitor) { throw common::exception::Exception("Unimplemented"); }
}  // namespace pagoda::objects
