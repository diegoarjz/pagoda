#ifndef PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_
#define PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_

#include "pagoda/objects/procedural_component.h"

#include <list>
#include <string>

namespace pagoda::objects
{
class HierarchicalComponent : public ProceduralComponent, public std::enable_shared_from_this<HierarchicalComponent>
{
	public:
	static std::string GetComponentSystemName();

	virtual ~HierarchicalComponent(){};

	std::string GetType() const override { return GetComponentSystemName(); }

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
}  // namespace pagoda::objects

#endif
