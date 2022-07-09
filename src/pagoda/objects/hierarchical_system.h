#ifndef PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_SYSTEM_H_
#define PAGODA_PROCEDURAL_OBJECTS_HIERARCHICAL_SYSTEM_H_

#include "pagoda/objects/hierarchical_component.h"
#include "pagoda/objects/procedural_component_system.h"
#include "pagoda/objects/procedural_object.h"
#include "pagoda/objects/procedural_operation.h"

namespace pagoda
{
class Pagoda;
}

namespace pagoda::objects
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

	/**
	 * Performs the registration of the Hierarchical System with \p pagoda.
	 */
	static void Registration(Pagoda *pagoda);

	protected:
	void DoClone(std::shared_ptr<HierarchicalComponent> from, std::shared_ptr<HierarchicalComponent> to) override;

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

}  // namespace pagoda::objects

#endif
