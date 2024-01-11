#include "pagoda/image/image.h"

#include <pagoda/image/core/image_implementation_factory.h>
#include <pagoda/image/io/image_reader_factory.h>

#include <memory>

using namespace boost;

namespace pagoda::image {
using namespace core;
using namespace io;

std::unique_ptr<ImageImplementationFactory> Image::s_implementationFactory;
std::unique_ptr<ImageReaderFactory> Image::s_imageReaderFactory;

Image::Image(const Dimensions &d, Format format)
    : m_impl{GetImageImplementationFactory()->Create(format)} {
  m_impl->Reserve(d);
}

Image::Image(std::unique_ptr<core::ImageImplementationBase> &&image)
    : m_impl{std::move(image)} {}

Image::Dimensions Image::GetDimensions() const {
  return m_impl->GetDimensions();
}

bool Image::IsLoaded() const { return m_impl->IsLoaded(); }

void Image::CopyImageData(std::vector<uint8_t> &data) {
  m_impl->CopyImageData(data);
}

Image::Pixel Image::GetPixel(std::size_t x, std::size_t y) const {
  return m_impl->GetPixel(x, y);
}

void Image::SetPixel(std::size_t x, std::size_t y, const Pixel &p) {
  m_impl->SetPixel(x, y, p);
}

ImageImplementationFactory *Image::GetImageImplementationFactory() {
  if (s_implementationFactory == nullptr) {
    s_implementationFactory = std::make_unique<ImageImplementationFactory>();
  }

  return s_implementationFactory.get();
}

ImageReaderFactory *Image::GetImageReaderFactory() {
  if (s_imageReaderFactory == nullptr) {
    s_imageReaderFactory = std::make_unique<ImageReaderFactory>();
  }

  return s_imageReaderFactory.get();
}

std::shared_ptr<Image> Image::CreateFromFile(const std::filesystem::path &file,
                                             Format format) {
  auto reader = GetImageReaderFactory()->CreateFromFilePath(file);
  return reader->Read(format);
}

} // namespace pagoda::image
