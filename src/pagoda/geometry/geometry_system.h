#ifndef PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_
#define PAGODA_PROCEDURAL_OBJECTS_GEOMETRY_SYSTEM_H_

#include "geometry_component.h"

#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>

#include <pagoda/objects/procedural_component_system.h>
#include <pagoda/objects/procedural_object.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda
{
class Pagoda;

namespace objects
{
class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;
}  // namespace objects
}  // namespace pagoda

namespace pagoda::geometry
{
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

	/**
	 * Performs the registration of the Geometry System in \p pagoda.
	 */
	static void Registration(Pagoda* pagoda);

	protected:
	void DoClone(std::shared_ptr<GeometryComponent> from, std::shared_ptr<GeometryComponent> to) override;

};  // class GeometrySystem
using GeometrySystemPtr = std::shared_ptr<GeometrySystem>;
using GeometrySystemWeakPtr = std::weak_ptr<GeometrySystem>;

}  // namespace pagoda::geometry

#endif
