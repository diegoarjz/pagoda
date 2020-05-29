#include "procedural_object_system.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>
#include <memory>

#include "procedural_component_system.h"
#include "procedural_object.h"

namespace pagoda::objects
{
ProceduralObjectSystem::ProceduralObjectSystem() { LOG_TRACE(Core, "Creating ProceduralObjectSystem"); }

ProceduralObjectSystem::~ProceduralObjectSystem() { LOG_TRACE(Core, "Destroying ProceduralObjectSystem"); }

std::shared_ptr<ProceduralObject> ProceduralObjectSystem::CreateProceduralObject()
{
	START_PROFILE;

	auto object = std::make_shared<ProceduralObject>();
	m_proceduralObjects.insert(object);

	return object;
}

std::shared_ptr<ProceduralObject> ProceduralObjectSystem::CloneProceduralObject(
    std::shared_ptr<ProceduralObject>& toClone)
{
	START_PROFILE;

	auto object = CreateProceduralObject();

	for (auto& system : m_proceduralComponentSystems)
	{
		system.second->CloneComponent(toClone, object);
	}

	return object;
}

bool ProceduralObjectSystem::RegisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystemBase> system)
{
	START_PROFILE;

	LOG_TRACE(Core, "Registering ProceduralComponentSystem with name " << system->GetComponentSystemTypeName().c_str());
	auto system_type = system->GetComponentSystemTypeName();

	if (m_proceduralComponentSystems.find(system_type) != std::end(m_proceduralComponentSystems))
	{
		return false;
	}

	m_proceduralComponentSystems.insert(std::make_pair(system_type, system));
	return true;
}

bool ProceduralObjectSystem::UnregisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystemBase> system)
{
	START_PROFILE;

	auto system_type = system->GetComponentSystemTypeName();

	DBG_ASSERT_MSG(m_proceduralObjects.size() == 0,
	               "Unregistering a ComponentSystem while there are procedural objects may cause incorrect behaviour.");

	auto iteratorToSystems = m_proceduralComponentSystems.find(system_type);
	if (iteratorToSystems == std::end(m_proceduralComponentSystems))
	{
		return false;
	}

	m_proceduralComponentSystems.erase(iteratorToSystems);
	return true;
}

std::shared_ptr<ProceduralComponentSystemBase> ProceduralObjectSystem::GetComponentSystem(const std::string& systemName)
{
	START_PROFILE;

	auto iteratorToSystems = m_proceduralComponentSystems.find(systemName);

	if (iteratorToSystems == std::end(m_proceduralComponentSystems))
	{
		return nullptr;
	}

	return iteratorToSystems->second;
}

void ProceduralObjectSystem::KillProceduralObject(std::shared_ptr<ProceduralObject>& proceduralObject)
{
	START_PROFILE;

	if (proceduralObject == nullptr)
	{
		return;
	}

	for (auto& system : m_proceduralComponentSystems)
	{
		system.second->KillProceduralComponent(proceduralObject);
	}

	m_proceduralObjects.erase(proceduralObject);
}
}  // namespace pagoda::objects
