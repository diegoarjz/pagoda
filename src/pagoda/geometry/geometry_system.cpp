#include "geometry_system.h"

#include <pagoda/geometry/geometry_component.h>

#include <pagoda/objects/procedural_object.h>

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

namespace pagoda::geometry
{
const std::string GeometrySystem::GetComponentSystemName() { return "GeometrySystem"; }

GeometrySystem::GeometrySystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
GeometrySystem::~GeometrySystem() {}

}  // namespace pagoda::geometry
