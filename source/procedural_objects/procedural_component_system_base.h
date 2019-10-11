#ifndef SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_BASE_H_
#define SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_BASE_H_

#include <memory>
#include <string>

namespace selector
{
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class ProceduralComponent;
using ProceduralComponentPtr = std::shared_ptr<ProceduralComponent>;

/**
 * Base class for a \c ProceduralComponentSystem.
 */
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
}  // namespace selector
#endif
