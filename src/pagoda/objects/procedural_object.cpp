#include "procedural_object.h"

#include "pagoda/objects/procedural_object_system.h"

#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_visitor.h>

using namespace pagoda::dynamic;

namespace pagoda::objects {
const TypeInfoPtr ProceduralObject::s_typeInfo =
    std::make_shared<TypeInfo>("ProceduralObject");

ProceduralObject::ProceduralObject(const ProceduralObjectSystemWeakPtr &owner)
    : BuiltinClass(s_typeInfo), m_owner{owner} {}

ProceduralObject::~ProceduralObject() {}

ProceduralObjectPtr ProceduralObject::Clone() {
  auto sharedFromThis =
      std::dynamic_pointer_cast<ProceduralObject>(shared_from_this());
  return m_owner.lock()->CloneProceduralObject(sharedFromThis);
}

std::string ProceduralObject::ToString() const { return "<ProceduralObject>"; }

void ProceduralObject::AcceptVisitor(ValueVisitorBase &visitor) {
  visitor.Visit(*this);
}
} // namespace pagoda::objects
