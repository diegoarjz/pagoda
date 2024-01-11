#pragma once

#include "image_implementation.h"

#include <pagoda/image/image.h>

#include <pagoda/common/factory.h>

#include <boost/filesystem.hpp>

#include <memory>

namespace pagoda::image::core
{
class ImageImplementationFactory : public common::Factory<ImageImplementationBase, Image::Format>
{
	public:
	ImageImplementationFactory();
	virtual ~ImageImplementationFactory();
};
}  // namespace pagoda::image::core
