#ifndef SELECTOR_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_
#define SELECTOR_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_

#include "procedural_object.h"
#include "procedural_operation.h"

#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_topology.h>
#include <geometry_operations/create_rect.h>
#include <geometry_operations/extrusion.h>
#include <parameter/parameter.h>

namespace selector
{
// TODO: Maybe move these type defs to geometry core
using Geometry = GeometryT<SplitPointTopology<uint32_t>>;
using GeometryPtr = std::shared_ptr<Geometry>;
using GeometryBuilder = GeometryBuilderT<Geometry>;
using GeometryBuilderPtr = std::shared_ptr<GeometryBuilder>;

class GeometryComponent;

/**
 * @brief Manages \c GeometryComponent.
 *
 * All instances of \c GeometryComponent should be created and destroyed (killed) through this system.
 */
class GeometrySystem : public ProceduralComponentSystem
{
public:
	virtual ~GeometrySystem();

	ComponentType GetType() const override { return ComponentType::Geometry; }

	std::shared_ptr<GeometryComponent> CreateComponent(GeometryPtr geometry);
	std::shared_ptr<ProceduralComponent> CreateComponent() override;
	void KillProceduralComponent(std::shared_ptr<ProceduralComponent> component) override;

	const std::unordered_set<std::shared_ptr<GeometryComponent>>& GetComponents() const { return geometry_components; }

	// Avaliable Operations
	CreateRect<Geometry> GetCreateRect(const float& width, const float& height);
	Extrusion<Geometry> GetExtrude(const float& amount);

private:
	std::unordered_set<std::shared_ptr<GeometryComponent>> geometry_components;
};  // class GeometrySystem
using GeometrySystemPtr = std::shared_ptr<GeometrySystem>;
using GeometrySystemWeakPtr = std::weak_ptr<GeometrySystem>;

}  // namespace selector

#endif
