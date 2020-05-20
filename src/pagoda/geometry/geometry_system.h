#ifndef PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_
#define PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_

#include <pagoda/geometry/algorithms/create_rect.h>
#include <pagoda/geometry/algorithms/extrusion.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>

#include <pagoda/objects/procedural_component_system.h>
#include <pagoda/objects/procedural_object.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry
{
// TODO: Maybe move these type defs to geometry core
using Geometry = geometry::core::GeometryBase<>;
using GeometryPtr = std::shared_ptr<Geometry>;
using GeometryBuilder = geometry::core::GeometryBuilderT<Geometry>;
using GeometryBuilderPtr = std::shared_ptr<GeometryBuilder>;

class GeometryComponent;

/**
 * @brief Manages \c GeometryComponent.
 *
 * All instances of \c GeometryComponent should be created and destroyed (killed) through this system.
 */
class GeometrySystem : public objects::ProceduralComponentSystem<GeometryComponent>
{
public:
	static const std::string GetComponentSystemName();

	GeometrySystem();
	virtual ~GeometrySystem();

};  // class GeometrySystem
using GeometrySystemPtr = std::shared_ptr<GeometrySystem>;
using GeometrySystemWeakPtr = std::weak_ptr<GeometrySystem>;

}  // namespace pagoda::geometry

#endif
