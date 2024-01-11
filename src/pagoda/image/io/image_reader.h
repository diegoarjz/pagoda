#pragma once

#include <pagoda/image/image.h>

#include <boost/gil.hpp>
#include <boost/gil/typedefs.hpp>

#include <filesystem>

namespace pagoda::image::io {
class ImageReader {
public:
  virtual void SetFilePath(const std::filesystem::path &p) = 0;
  virtual void Read(boost::gil::rgb8_image_t &img) = 0;

  virtual std::shared_ptr<Image> Read(Image::Format f) = 0;
};

} // namespace pagoda::image::io
