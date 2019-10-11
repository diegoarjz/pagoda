#ifndef SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_
#define SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_

#include "common/assertions.h"
#include "common/logger.h"

#include "procedural_component_system_base.h"

#include <string>
#include <unordered_map>

namespace selector
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class ProceduralComponent;
using ProceduralComponentPtr = std::shared_ptr<ProceduralComponent>;

/**
 * Base template class for a \c ProceduralComponentSytem that holds \c ProceduralComponent
 * of type C.
 */
template<class C>
class ProceduralComponentSystem : public ProceduralComponentSystemBase
{
public:
    /// Type of \c ProceduralComponent managed by this \c ProceduralComponentSystem.
	using Component_t = C;

	ProceduralComponentSystem(const std::string &name) : ProceduralComponentSystemBase(name)
    {
    }

	virtual ~ProceduralComponentSystem()
    {
    }

    /**
     * Creates a \c ProceduralComponent of type \c Component_t for the \c ProceduralObject give in \p object.
     */
	std::shared_ptr<ProceduralComponent> CreateComponent(ProceduralObjectPtr object) override
	{
        DBG_ASSERT_MSG(object != nullptr, "Can't create a component (%s) for a null ProceduralObject", GetComponentSystemTypeName().c_str());
		DBG_ASSERT_MSG(m_components.find(object) == std::end(m_components),
		               "Procedural object already has a component for %s", GetComponentSystemTypeName().c_str());

		auto component = std::make_shared<Component_t>();
		m_components[object] = component;
		return component;
	}

    /**
     * Returns the \c ProceduralComponent for the \c ProceduralObject \p object.
     */
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

    /**
     * Deletes the \c ProceduralComponent for the \c ProceduralObject \p object.
     */
	void KillProceduralComponent(ProceduralObjectPtr object) override { m_components.erase(object); }

private:
    /// Stores the \c ProceduralComponent for each \c ProceduralObject.
	std::unordered_map<ProceduralObjectPtr, std::shared_ptr<Component_t>> m_components;
};  // class ProceduralComponentSystem

}  // namespace selector

#endif
