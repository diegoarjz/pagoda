#ifndef SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_
#define SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_

#include "common/assertions.h"
#include "common/logger.h"

#include <string>
#include <unordered_map>

namespace selector
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class ProceduralComponent;
using ProceduralComponentPtr = std::shared_ptr<ProceduralComponent>;

class ProceduralComponentSystemBase
{
public:
	ProceduralComponentSystemBase(const std::string &name);
	virtual ~ProceduralComponentSystemBase();

	std::string GetComponentSystemTypeName() const;
	virtual std::shared_ptr<ProceduralComponent> CreateComponent(ProceduralObjectPtr) = 0;
	virtual std::shared_ptr<ProceduralComponent> GetComponent(ProceduralObjectPtr object) = 0;
	virtual void KillProceduralComponent(ProceduralObjectPtr) = 0;

	template<typename C>
	std::shared_ptr<C> GetComponentAs(ProceduralObjectPtr object)
	{
		return std::dynamic_pointer_cast<C>(GetComponent(object));
	}

	template<typename C>
	std::shared_ptr<C> CreateComponentAs(ProceduralObjectPtr object)
	{
		return std::dynamic_pointer_cast<C>(CreateComponent(object));
	}

private:
	const std::string m_systemName;
};

template<class C>
class ProceduralComponentSystem : public ProceduralComponentSystemBase
{
public:
	using Component_t = C;

	ProceduralComponentSystem(const std::string &name) : ProceduralComponentSystemBase(name)
    {
    }

	virtual ~ProceduralComponentSystem()
    {
    }

	std::shared_ptr<ProceduralComponent> CreateComponent(ProceduralObjectPtr object) override
	{
        DBG_ASSERT_MSG(object != nullptr, "Can't create a component (%s) for a null ProceduralObject", GetComponentSystemTypeName().c_str());
		DBG_ASSERT_MSG(m_components.find(object) == std::end(m_components),
		               "Procedural object already has a component for %s", GetComponentSystemTypeName().c_str());

		auto component = std::make_shared<Component_t>();
		m_components[object] = component;
		return component;
	}

	std::shared_ptr<ProceduralComponent> GetComponent(ProceduralObjectPtr object) override
	{
        DBG_ASSERT_MSG(object != nullptr, "Can't get a component (%s) for a null ProceduralObject", GetComponentSystemTypeName().c_str());
		auto component = m_components.find(object);
		if (component == std::end(m_components))
		{
			return nullptr;
		}
		return component->second;
	}

	void KillProceduralComponent(ProceduralObjectPtr object) override { m_components.erase(object); }

private:
	std::unordered_map<ProceduralObjectPtr, std::shared_ptr<Component_t>> m_components;
};  // class ProceduralComponentSystem

}  // namespace selector

#endif
