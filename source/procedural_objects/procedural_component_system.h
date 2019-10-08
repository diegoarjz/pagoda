#ifndef SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_
#define SELECTOR_PROCEDURAL_COMPONENT_SYSTEM_H_

#include "procedural_component.h"

#include <string>

namespace selector
{
class ProceduralComponent;

class ProceduralComponentSystem
{
public:
	ProceduralComponentSystem(const std::string &name);
	~ProceduralComponentSystem();

	virtual std::shared_ptr<ProceduralComponent> CreateComponent() = 0;
	virtual void KillProceduralComponent(std::shared_ptr<ProceduralComponent> component) = 0;
	virtual ComponentType GetType() const = 0;

private:
	const std::string &m_systemName;
};  // class ProceduralComponentSystem

}  // namespace selector

#endif
