#ifndef SELECTOR_PROCEDURAL_OBJECTS_HIERARCHICAL_SYSTEM_H_
#define SELECTOR_PROCEDURAL_OBJECTS_HIERARCHICAL_SYSTEM_H_

#include "procedural_object.h"
#include "procedural_operation.h"
#include "procedural_component_system.h"

namespace selector
{
class HierarchicalComponent;

class HierarchicalSystem : public ProceduralComponentSystem<HierarchicalComponent>
{
public:
    using Component_t = HierarchicalComponent;

	static const std::string GetComponentSystemName();

    HierarchicalSystem();
	virtual ~HierarchicalSystem();

	void SetParent(std::shared_ptr<HierarchicalComponent> parent, std::shared_ptr<HierarchicalComponent> child);

private:
	struct HierarchicalComponentWeakPtrHasher
	{
		size_t operator()(const std::weak_ptr<HierarchicalComponent> &ptr) const
		{
			return std::hash<std::shared_ptr<HierarchicalComponent>>()(ptr.lock());
		}
	};  // struct HierarchicalComponentWeakPtrHasher

	struct HierarchicalComponentEqual
	{
		bool operator()(const std::weak_ptr<HierarchicalComponent> &lhs,
		                const std::weak_ptr<HierarchicalComponent> &rhs) const
		{
			return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
		}
	};  // struct HierarchicalComponentEqual

	std::unordered_set<std::weak_ptr<HierarchicalComponent>, HierarchicalComponentWeakPtrHasher,
	                   HierarchicalComponentEqual>
	    root_components;
};  // class GeometrySystem
using HierarchicalSystemPtr = std::shared_ptr<HierarchicalSystem>;
using HierarchicalSystemWeakPtr = std::weak_ptr<HierarchicalSystem>;

}  // namespace selector

#endif
