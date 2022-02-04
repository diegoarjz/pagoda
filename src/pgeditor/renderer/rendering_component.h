#pragma once

#include <pagoda/objects/procedural_component.h>

namespace pgeditor::renderer {

using RenderablePtr = std::shared_ptr<class Renderable>;

/**
 * A RenderingComponent for ProceduralObject.
 */
class RenderingComponent : public pagoda::objects::ProceduralComponent {
public:
  static std::string GetComponentSystemName();

  std::string GetType() const override { return GetComponentSystemName(); }

  ~RenderingComponent() override {}

  void SetRenderable(const RenderablePtr& renderable);
  const RenderablePtr& GetRenderable() const;

private:
  RenderablePtr m_renderable;
};

using RenderingComponentPtr = std::shared_ptr<RenderingComponent>;
} // namespace pgeditor::renderer
