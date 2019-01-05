#ifndef SELECTOR_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_
#define SELECTOR_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_

#include "procedural_component.h"

#include <list>

namespace selector
{
class HierarchicalComponent : public ProceduralComponent, public std::enable_shared_from_this<HierarchicalComponent>
{
public:
	static ComponentType GetType() { return ComponentType::Hierarchical; }

	virtual ~HierarchicalComponent(){};

	ComponentType Type() const override { return GetType(); }

	size_t ChildrenCount() const { return children.size(); }
	std::shared_ptr<HierarchicalComponent> GetParent() const { return parent.lock(); }
	std::list<std::weak_ptr<HierarchicalComponent>>::const_iterator cbegin() { return children.cbegin(); }
	std::list<std::weak_ptr<HierarchicalComponent>>::const_iterator cend() { return children.cend(); }

	friend class HierarchicalSystem;

private:
	void SetParent(std::shared_ptr<HierarchicalComponent> parent);

	std::weak_ptr<HierarchicalComponent> parent;
	std::list<std::weak_ptr<HierarchicalComponent>> children;
};  // class HierarchicalComponent
}  // namespace selector

#endif
