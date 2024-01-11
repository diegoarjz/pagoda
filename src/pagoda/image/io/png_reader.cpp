#include "png_reader.h"

#include <pagoda/image/core/rgb8_image.h>

#include <pagoda/image/image.h>

#include <pagoda/common/debug/logger.h>

#include <boost/gil/extension/io/png/old.hpp>
#include <boost/gil/extension/io/png/read.hpp>
#include <boost/gil/io/read_and_convert_view.hpp>
#include <boost/gil/io/read_image.hpp>
#include <boost/gil/typedefs.hpp>

#include <memory>

using namespace boost;

namespace pagoda::image::io {
PngReader::PngReader() {}
PngReader::~PngReader() {}

void PngReader::SetFilePath(const std::filesystem::path &p) { m_filePath = p; }
void PngReader::Read(boost::gil::rgb8_image_t &img) {
  gil::png_read_and_convert_image(m_filePath.string(), img);
}

namespace {
template <class ImageData, class ImageType>
std::shared_ptr<Image> createImage(const std::filesystem::path &filePath) {
  auto img = std::make_unique<typename ImageType::ImageData_t>();
  gil::png_read_and_convert_image(filePath.string(), *img);
  auto imgImpl = std::make_unique<ImageType>(std::move(img));
  return std::make_shared<Image>(std::move(imgImpl));
}
} // namespace

std::shared_ptr<Image> PngReader::Read(Image::Format f) {
  switch (f) {
  case Image::Format::RGB8: {
    return createImage<gil::rgb8_image_t, core::RGB8Image>(m_filePath);
  }
  case Image::Format::RGBA8:
    return createImage<gil::rgba8_image_t, core::RGBA8Image>(m_filePath);
    break;
  };
  return nullptr;
}
} // namespace pagoda::image::io
