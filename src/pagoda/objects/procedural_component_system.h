#ifndef PAGODA_PROCEDURAL_COMPONENT_SYSTEM_H_
#define PAGODA_PROCEDURAL_COMPONENT_SYSTEM_H_

#include "procedural_component_system_base.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace pagoda::objects
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

	ProceduralComponentSystem(const std::string &name) : ProceduralComponentSystemBase(name) {}

	virtual ~ProceduralComponentSystem() {}

	/**
	 * Creates a \c ProceduralComponent of type \c Component_t for the \c ProceduralObject give in \p object.
	 */
	std::shared_ptr<ProceduralComponent> CreateComponent(ProceduralObjectPtr object) override
	{
		DBG_ASSERT_MSG(object != nullptr, "Can't create a component (%s) for a null ProceduralObject",
		               GetComponentSystemTypeName().c_str());

		auto component = std::make_shared<Component_t>();
		m_components[object] = component;
		return std::static_pointer_cast<ProceduralComponent>(component);
	}

	/**
	 * Returns the \c ProceduralComponent for the \c ProceduralObject \p object.
	 */
	std::shared_ptr<ProceduralComponent> GetComponent(ProceduralObjectPtr object) override
	{
		DBG_ASSERT_MSG(object != nullptr, "Can't get a component (%s) for a null ProceduralObject",
		               GetComponentSystemTypeName().c_str());
		auto component = m_components.find(object);
		if (component == std::end(m_components)) {
			return nullptr;
		}
		return std::static_pointer_cast<ProceduralComponent>(component->second);
	}

	void CloneComponent(ProceduralObjectPtr from, ProceduralObjectPtr to) override
	{
		auto originalComponent = GetComponentAs<Component_t>(from);
		if (originalComponent == nullptr) {
			// no component to be cloned so nothing to be done
			return;
		}
		auto newComponent = CreateComponentAs<Component_t>(to);
		DoClone(originalComponent, newComponent);
	}

	/**
	 * Deletes the \c ProceduralComponent for the \c ProceduralObject \p object.
	 */
	void KillProceduralComponent(ProceduralObjectPtr object) override { m_components.erase(object); }

	protected:
	virtual void DoClone(std::shared_ptr<Component_t> from, std::shared_ptr<Component_t> to) = 0;

	/// Stores the \c ProceduralComponent for each \c ProceduralObject.
	std::unordered_map<ProceduralObjectPtr, std::shared_ptr<Component_t>> m_components;
};  // class ProceduralComponentSystem

}  // namespace pagoda::objects

#endif
