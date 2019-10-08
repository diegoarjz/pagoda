#ifndef SELECTOR_PROCEDURAL_OBJECT_SYSTEM_H_
#define SELECTOR_PROCEDURAL_OBJECT_SYSTEM_H_

#include "procedural_object_mask.h"
#include "common/assertions.h"
#include "common/profiler.h"

#include <memory>
#include <unordered_set>
#include <unordered_map>

namespace selector
{
class ProceduralObject;
class ProceduralComponentSystem;

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
	std::shared_ptr<ProceduralObject> CreateProceduralObject(const ProceduralObjectMask& component_mask);
	void KillProceduralObject(std::shared_ptr<ProceduralObject>& proceduralObject);

	bool RegisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystem> system);
	bool UnregisterProceduralComponentSystem(std::shared_ptr<ProceduralComponentSystem> system);

	template<class System>
	bool UnregisterProceduralComponentSystem()
	{
		DBG_ASSERT_MSG(
		    procedural_objects.size() == 0,
		    "Unregistering a ComponentSystem while there are procedural objects may cause incorrect behaviour.");
		START_PROFILE;

		ComponentType type = System::GetType();

		auto iterator_to_systems = procedural_component_systems.find(type);
		if (iterator_to_systems == std::end(procedural_component_systems))
		{
			return false;
		}

		procedural_component_systems.erase(iterator_to_systems);
		return true;
	}

	std::shared_ptr<ProceduralComponentSystem> GetComponentSystem(const ComponentType& type);

	const std::unordered_set<std::shared_ptr<ProceduralObject>>& GetProceduralObjects() const
	{
		return procedural_objects;
	}

private:
	std::unordered_set<std::shared_ptr<ProceduralObject>> procedural_objects;
	std::unordered_map<ComponentType, std::shared_ptr<ProceduralComponentSystem>> procedural_component_systems;
};  // class ProceduralObjectSystem

using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;
using ProceduralObjectSystemWeakPtr = std::weak_ptr<ProceduralObjectSystem>;

}  // namespace selector

#endif
