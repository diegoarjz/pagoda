#pragma once

#include "rendering_component.h"

#include <pagoda/objects/procedural_component_system.h>

namespace pagoda
{
class Pagoda;
}

namespace pgeditor::renderer
{
class RenderingSystem : public pagoda::objects::ProceduralComponentSystem<RenderingComponent>
{
	public:
	static const std::string GetComponentSystemName();
	static void Registration(pagoda::Pagoda* pagoda);

	RenderingSystem();
	~RenderingSystem() override {}

	protected:
	void DoClone(std::shared_ptr<Component_t> from, std::shared_ptr<Component_t> to) override;

	private:
};
}  // namespace pgeditor::renderer
