#ifndef PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_
#define PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_COMPONENT_H_

#include "pagoda/objects/procedural_component.h"

#include <list>
#include <string>

namespace pagoda::objects
{
using HierarchicalComponentPtr = std::shared_ptr<class HierarchicalComponent>;

class HierarchicalComponent : public ProceduralComponent, public std::enable_shared_from_this<HierarchicalComponent>
{
	public:
  using ChildrenContainer_t = std::list<std::weak_ptr<HierarchicalComponent>>;

	static std::string GetComponentSystemName();

	virtual ~HierarchicalComponent(){};

	std::string GetType() const override { return GetComponentSystemName(); }

	size_t ChildrenCount() const { return children.size(); }
	std::shared_ptr<HierarchicalComponent> GetParent() const { return parent.lock(); }
  const ChildrenContainer_t& GetChildren() const { return children; }
	std::list<std::weak_ptr<HierarchicalComponent>>::const_iterator cbegin() { return children.cbegin(); }
	std::list<std::weak_ptr<HierarchicalComponent>>::const_iterator cend() { return children.cend(); }

	friend class HierarchicalSystem;

	private:
	void SetParent(HierarchicalComponentPtr parent);

	std::weak_ptr<HierarchicalComponent> parent;
	std::list<std::weak_ptr<HierarchicalComponent>> children;
};  // class HierarchicalComponent
}  // namespace pagoda::objects

#endif
