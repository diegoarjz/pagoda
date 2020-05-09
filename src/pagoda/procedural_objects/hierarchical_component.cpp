#include "hierarchical_component.h"

#include "common/profiler.h"
#include "hierarchical_system.h"

namespace pagoda
{
std::string HierarchicalComponent::GetComponentSystemName() { return HierarchicalSystem::GetComponentSystemName(); }

void HierarchicalComponent::SetParent(std::shared_ptr<HierarchicalComponent> parent)
{
	START_PROFILE;

	if (this->parent.lock() != nullptr)
	{
		// Remove from children
		this->parent.lock()->children.remove_if(
		    [this](std::weak_ptr<HierarchicalComponent> wp) { return this == wp.lock().get(); });
	}

	this->parent = parent;

	if (this->parent.lock() != nullptr)
	{
		this->parent.lock()->children.push_back(shared_from_this());
	}
}

}  // namespace pagoda
