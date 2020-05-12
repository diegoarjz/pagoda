#include "geometry_system.h"

#include "geometry_component.h"
#include "procedural_object.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

namespace pagoda
{
const std::string GeometrySystem::GetComponentSystemName() { return "GeometrySystem"; }

GeometrySystem::GeometrySystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
GeometrySystem::~GeometrySystem() {}

}  // namespace pagoda
