#pragma once

#include "image_reader.h"

#include <pagoda/common/factory.h>

#include <boost/filesystem/path.hpp>
#include <memory>

namespace pagoda::image::io
{
class ImageReaderFactory : public common::Factory<ImageReader>
{
public:
	ImageReaderFactory();
	virtual ~ImageReaderFactory();

	std::shared_ptr<ImageReader> CreateFromFilePath(const boost::filesystem::path &p);

private:
};
}  // namespace pagoda::image::io
