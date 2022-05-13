#include "hierarchical_system.h"

#include <pagoda/pagoda.h>
#include "hierarchical_component.h"

#include "pagoda/objects/operation_factory.h"
#include "pagoda/objects/procedural_object_system.h"

#include <memory>

namespace pagoda::objects
{
const std::string HierarchicalSystem::GetComponentSystemName() { return "HierarchicalSystem"; }

HierarchicalSystem::HierarchicalSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
HierarchicalSystem::~HierarchicalSystem() {}

void HierarchicalSystem::SetParent(std::shared_ptr<HierarchicalComponent> parent,
                                   std::shared_ptr<HierarchicalComponent> child)
{
	DBG_ASSERT_MSG(child != nullptr, "Child component must not be null");

	child->SetParent(parent);

	if (parent != nullptr) {
		// remove child from root nodes
		root_components.erase(child);
	} else {
		// add child to root nodes
		root_components.insert(child);
	}
}

void HierarchicalSystem::DoClone(std::shared_ptr<HierarchicalComponent> from, std::shared_ptr<HierarchicalComponent> to)
{
	to->parent = from->parent;
	to->children = from->children;
}

void HierarchicalSystem::Registration(Pagoda* pagoda)
{
	LOG_TRACE(Core, "Registering " << GetComponentSystemName());
	auto objectSystem = pagoda->GetProceduralObjectSystem();

	objectSystem->RegisterProceduralComponentSystem(std::make_shared<HierarchicalSystem>());
}

}  // namespace pagoda::objects
