#include "procedural_object_system.h"

#include "common/logger.h"
#include "common/profiler.h"
#include "common/assertions.h"

#include "procedural_component_system.h"
#include "procedural_object.h"

namespace selector
{
ProceduralObjectSystem::ProceduralObjectSystem()
{
    LOG_TRACE(Core, "Creating ProceduralObjectSystem");
}

ProceduralObjectSystem::~ProceduralObjectSystem()
{
    LOG_TRACE(Core, "Destroying ProceduralObjectSystem");
	DBG_ASSERT_MSG(procedural_objects.size() == 0,
	               "ProceduralObjectSystem still has procedural objects on destruction.");
}

std::shared_ptr<ProceduralObject> ProceduralObjectSystem::CreateProceduralObject()
{
	START_PROFILE;

	auto object = std::make_shared<ProceduralObject>();
	procedural_objects.insert(object);

	return object;
}

std::shared_ptr<ProceduralObject> ProceduralObjectSystem::CreateProceduralObject(
    const ProceduralObjectMask& component_mask)
{
	START_PROFILE;

	auto procedural_object = CreateProceduralObject();

	for (auto i = 0u; i < component_mask.size(); ++i)
	{
		if (component_mask[i])
		{
			auto component_system = GetComponentSystem(static_cast<ComponentType>(i));
			DBG_ASSERT_MSG(component_system != nullptr,
			               "Trying to create a procedural object with components for unregistered systems");
			auto component = component_system->CreateComponent();
			procedural_object->SetComponent(static_cast<ComponentType>(i), component);
		}
	}

	return procedural_object;
}

bool ProceduralObjectSystem::RegisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystem> system)
{
	START_PROFILE;

	auto system_type = system->GetType();

	if (procedural_component_systems.find(system_type) != std::end(procedural_component_systems))
	{
		return false;
	}

	procedural_component_systems.insert(std::make_pair(system_type, system));
	return true;
}

bool ProceduralObjectSystem::UnregisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystem> system)
{
	START_PROFILE;

	auto system_type = system->GetType();

	DBG_ASSERT_MSG(procedural_objects.size() == 0,
	               "Unregistering a ComponentSystem while there are procedural objects may cause incorrect behaviour.");

	auto iterator_to_systems = procedural_component_systems.find(system_type);
	if (iterator_to_systems == std::end(procedural_component_systems))
	{
		return false;
	}

	procedural_component_systems.erase(iterator_to_systems);
	return true;
}

std::shared_ptr<ProceduralComponentSystem> ProceduralObjectSystem::GetComponentSystem(const ComponentType& type)
{
	START_PROFILE;

	auto iterator_to_systems = procedural_component_systems.find(type);

	if (iterator_to_systems == std::end(procedural_component_systems))
	{
		return nullptr;
	}

	return iterator_to_systems->second;
}

void ProceduralObjectSystem::KillProceduralObject(std::shared_ptr<ProceduralObject>& proceduralObject)
{
	START_PROFILE;

	if (proceduralObject == nullptr)
	{
		return;
	}

	const std::vector<ProceduralComponentWeakPtr>& components = proceduralObject->GetComponents();

	for (const ProceduralComponentWeakPtr& c : components)
	{
		if (c.expired())
		{
			continue;
		}

		auto iterator_to_systems = procedural_component_systems.find(static_cast<ComponentType>(c.lock()->Type()));

		DBG_ASSERT_MSG(iterator_to_systems != std::end(procedural_component_systems),
		               "Killing a procedural object that has components for unregistered systems");

		iterator_to_systems->second->KillProceduralComponent(c.lock());
	}

	procedural_objects.erase(proceduralObject);
}
}  // namespace selector
