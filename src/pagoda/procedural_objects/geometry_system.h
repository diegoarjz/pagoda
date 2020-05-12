#ifndef PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_
#define PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_

#include "procedural_component_system.h"
#include "procedural_object.h"
#include "procedural_operation.h"

#include <pagoda/geometry_core/geometry.h>
#include <pagoda/geometry_core/geometry_builder.h>
#include <pagoda/geometry_operations/create_rect.h>
#include <pagoda/geometry_operations/extrusion.h>

namespace pagoda
{
// TODO: Maybe move these type defs to geometry core
using Geometry = GeometryBase<>;
using GeometryPtr = std::shared_ptr<Geometry>;
using GeometryBuilder = GeometryBuilderT<Geometry>;
using GeometryBuilderPtr = std::shared_ptr<GeometryBuilder>;

class GeometryComponent;

/**
 * @brief Manages \c GeometryComponent.
 *
 * All instances of \c GeometryComponent should be created and destroyed (killed) through this system.
 */
class GeometrySystem : public ProceduralComponentSystem<GeometryComponent>
{
public:
	static const std::string GetComponentSystemName();

	GeometrySystem();
	virtual ~GeometrySystem();

};  // class GeometrySystem
using GeometrySystemPtr = std::shared_ptr<GeometrySystem>;
using GeometrySystemWeakPtr = std::weak_ptr<GeometrySystem>;

}  // namespace pagoda

#endif
