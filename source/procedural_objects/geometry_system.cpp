#include "geometry_system.h"

#include "common/assertions.h"
#include "common/profiler.h"
#include "geometry_component.h"
#include "procedural_object.h"

namespace pagoda
{
const std::string GeometrySystem::GetComponentSystemName() { return "GeometrySystem"; }

GeometrySystem::GeometrySystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
GeometrySystem::~GeometrySystem() {}

}  // namespace pagoda
