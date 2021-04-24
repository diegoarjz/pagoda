#include "rendering_system.h"

#include <pagoda/pagoda.h>

namespace pgeditor::renderer
{
const std::string RenderingSystem::GetComponentSystemName() { return "RenderingSystem"; }

void RenderingSystem::Registration(pagoda::Pagoda* pagoda)
{
	auto operationFactory = pagoda->GetOperationFactory();
	auto objectSystem = pagoda->GetProceduralObjectSystem();

	objectSystem->RegisterProceduralComponentSystem(std::make_shared<RenderingSystem>());
}

RenderingSystem::RenderingSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}

void RenderingSystem::DoClone(std::shared_ptr<RenderingComponent> from, std::shared_ptr<RenderingComponent> to)
{
	//
}
}  // namespace pgeditor::renderer
