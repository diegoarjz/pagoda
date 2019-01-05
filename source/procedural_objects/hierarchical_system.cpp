#include "hierarchical_system.h"

#include "hierarchical_component.h"

namespace selector
{
HierarchicalSystem::~HierarchicalSystem() {}

std::shared_ptr<ProceduralComponent> HierarchicalSystem::CreateComponent()
{
	START_PROFILE;

	auto hierarchical_component = std::make_shared<HierarchicalComponent>();
	hierarchical_components.insert(hierarchical_component);
	root_components.insert(hierarchical_component);
	return std::dynamic_pointer_cast<ProceduralComponent>(hierarchical_component);
}

void HierarchicalSystem::KillProceduralComponent(std::shared_ptr<ProceduralComponent> component)
{
	START_PROFILE;

	DBG_ASSERT_MSG(std::dynamic_pointer_cast<HierarchicalComponent>(component) != nullptr,
	               "Trying to kill a component that is not a geometry component");
	hierarchical_components.erase(std::dynamic_pointer_cast<HierarchicalComponent>(component));
}

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
