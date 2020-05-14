#include "procedural_component.h"

namespace pagoda::objects
{
void ProceduralComponent::SetParentObject(ProceduralObjectPtr parent) { parent_object = parent; }
ProceduralObjectPtr ProceduralComponent::GetParentObject() { return parent_object.lock(); }
}  // namespace pagoda::objects
