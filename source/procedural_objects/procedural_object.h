#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OBJECT_H_

#include "procedural_component.h"

#include <geometry_core/geometry.h>

#include <bitset>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace selector
{
using ProceduralObjectMask = std::bitset<static_cast<size_t>(ComponentType::MaxComponents)>;

class ProceduralComponentSystem
{
public:
	virtual std::shared_ptr<ProceduralComponent> CreateComponent() = 0;
	virtual void KillProceduralComponent(std::shared_ptr<ProceduralComponent> component) = 0;
	virtual ComponentType GetType() const = 0;
};  // class ProceduralComponentSystem

/**
 * Manages all \c ProceduralObject.
 *
 * All \c ProceduralObject should be created and killed through a \c ProceduralObjectSystem.
 */
class ProceduralObjectSystem
{
public:
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

class ProceduralObject : public std::enable_shared_from_this<ProceduralObject>
{
public:
	ProceduralObject();

	const std::vector<ProceduralComponentWeakPtr>& GetComponents() const { return components; }
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		return std::dynamic_pointer_cast<T>(components[static_cast<size_t>(T::GetType())].lock());
	}

	void SetComponent(const ComponentType& type, std::shared_ptr<ProceduralComponent> component);

	template<class T>
	void SetComponent(std::shared_ptr<T> component)
	{
		CRITICAL_ASSERT_MSG(component != nullptr, "Setting a null component");
		components[static_cast<size_t>(T::GetType())] = component;
		component->SetParentObject(shared_from_this());
		procedural_object_mask.set(static_cast<size_t>(T::GetType()), true);
	}

	template<class T>
	ProceduralComponentPtr RemoveComponent()
	{
		auto component = components[static_cast<size_t>(T::GetType())];
		component.lock()->SetParentObject(nullptr);
		components[static_cast<size_t>(T::GetType())].reset();
		procedural_object_mask.set(static_cast<size_t>(T::GetType()), false);
		return component.lock();
	}

	const ProceduralObjectMask& Mask() const { return procedural_object_mask; }

private:
	// ID
	// Procedural Components
	std::vector<ProceduralComponentWeakPtr> components;
	ProceduralObjectMask procedural_object_mask;
};  // class ProceduralObject

using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
using ProceduralObjectWeakPtr = std::weak_ptr<ProceduralObject>;

}  // namespace selector

#endif
