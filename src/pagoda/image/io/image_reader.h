#pragma once

#include <boost/filesystem.hpp>
#include <boost/gil.hpp>
#include <boost/gil/typedefs.hpp>

namespace pagoda::image::io
{
class ImageReader
{
public:
	virtual void SetFilePath(const boost::filesystem::path& p) = 0;
	virtual void Read(boost::gil::rgb8_image_t& img) = 0;
};
}  // namespace pagoda::image::io
