#pragma once

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace pagoda::objects
{
class ProceduralObject;
class ProceduralComponentSystemBase;

/**
 * Manages all \c ProceduralObject.
 *
 * All \c ProceduralObject should be created and killed through a \c ProceduralObjectSystem.
 */
class ProceduralObjectSystem
{
public:
	ProceduralObjectSystem();
	~ProceduralObjectSystem();

	std::shared_ptr<ProceduralObject> CreateProceduralObject();
	void KillProceduralObject(std::shared_ptr<ProceduralObject>& proceduralObject);

	bool RegisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystemBase> system);
	bool UnregisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystemBase> system);

	template<class System>
	bool UnregisterProceduralComponentSystem()
	{
		DBG_ASSERT_MSG(
		    m_proceduralObjects.size() == 0,
		    "Unregistering a ComponentSystem while there are procedural objects may cause incorrect behaviour.");
		START_PROFILE;

		std::string type = System::GetComponentSystem();

		auto iteratorToSystems = m_proceduralComponentSystems.find(type);
		if (iteratorToSystems == std::end(m_proceduralComponentSystems))
		{
			return false;
		}

		m_proceduralComponentSystems.erase(iteratorToSystems);
		return true;
	}

	template<class System>
	std::shared_ptr<System> GetComponentSystem()
	{
		return std::dynamic_pointer_cast<System>(GetComponentSystem(System::GetComponentSystemName()));
	}

	std::shared_ptr<ProceduralComponentSystemBase> GetComponentSystem(const std::string& systemName);

	const std::unordered_set<std::shared_ptr<ProceduralObject>>& GetProceduralObjects() const
	{
		return m_proceduralObjects;
	}

private:
	std::unordered_set<std::shared_ptr<ProceduralObject>> m_proceduralObjects;
	std::unordered_map<std::string, std::shared_ptr<ProceduralComponentSystemBase>> m_proceduralComponentSystems;
};  // class ProceduralObjectSystem

using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;
using ProceduralObjectSystemWeakPtr = std::weak_ptr<ProceduralObjectSystem>;

}  // namespace pagoda::objects

