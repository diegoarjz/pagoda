#pragma once

#include "pagoda/image/image.h"

#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_component_system.h>

namespace pagoda{
  class Pagoda;
}

namespace pagoda::image {
class ImageComponent : public objects::ProceduralComponent {
public:
	static std::string GetComponentSystemName();

  ~ImageComponent() override = default;

	std::string GetType() const override { return GetComponentSystemName(); }

  void SetImage(ImagePtr i) { m_image = i; }
  ImagePtr GetImage() const { return m_image; }

private:
  std::shared_ptr<Image> m_image;
};

class ImageSystem : public objects::ProceduralComponentSystem<ImageComponent> {
public:
	static const std::string GetComponentSystemName();

  ImageSystem();
  ~ImageSystem() override = default;

	void DoClone(std::shared_ptr<ImageComponent> from, std::shared_ptr<ImageComponent> to) override;

	static void Registration(Pagoda* pagoda);

private:
};
} // namespace pagoda::image
