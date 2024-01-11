#include "image_implementation_factory.h"

#include "rgb8_image.h"

#include <memory>

namespace pagoda::image::core
{
ImageImplementationFactory::ImageImplementationFactory()
  : Factory<ImageImplementationBase, Image::Format>("ImageImplementation")
{
	Register(Image::Format::RGB8, []() { return std::make_shared<RGB8Image>(); });
}

ImageImplementationFactory::~ImageImplementationFactory() {}

}  // namespace pagoda::image::core
