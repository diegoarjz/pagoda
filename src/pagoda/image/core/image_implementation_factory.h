#pragma once

#include "image_implementation.h"

#include <pagoda/image/image_type.h>

#include <pagoda/common/factory.h>

#include <boost/filesystem.hpp>

#include <memory>

namespace pagoda::image::core
{
class ImageImplementationFactory : public common::Factory<ImageImplementation, ImageType>
{
	public:
	ImageImplementationFactory();
	virtual ~ImageImplementationFactory();
};
}  // namespace pagoda::image::core
