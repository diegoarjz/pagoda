#include "hierarchical_system.h"

#include "hierarchical_component.h"

namespace selector
{
const std::string HierarchicalSystem::GetComponentSystemName() { return "HierarchicalSystem"; }

HierarchicalSystem::HierarchicalSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
HierarchicalSystem::~HierarchicalSystem() {}

void HierarchicalSystem::SetParent(std::shared_ptr<HierarchicalComponent> parent,
                                   std::shared_ptr<HierarchicalComponent> child)
{
	DBG_ASSERT_MSG(child != nullptr, "Child component must not be null");

	child->SetParent(parent);

	if (parent != nullptr)
	{
		// remove child from root nodes
		root_components.erase(child);
	}
	else
	{
		// add child to root nodes
		root_components.insert(child);
	}
}

}  // namespace selector
