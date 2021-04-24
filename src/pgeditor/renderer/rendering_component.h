#pragma once

#include <pagoda/objects/procedural_component.h>

namespace pgeditor::renderer
{
class RenderingComponent : public pagoda::objects::ProceduralComponent
{
	public:
	static std::string GetComponentSystemName();

	std::string GetType() const override { return GetComponentSystemName(); }

	~RenderingComponent() override {}

	private:
};
}  // namespace pgeditor::renderer
