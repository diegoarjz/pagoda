#pragma once

#include "pagoda/image/image.h"

#include <memory>
#include <utility>
#include <vector>

namespace pagoda::image::io
{
class ImageReader;
}

namespace pagoda::image::core
{
class ImageImplementationBase
{
	public:

	virtual ~ImageImplementationBase() {}

  virtual bool Reserve(const Image::Dimensions& dimensions) = 0;

	virtual Image::Dimensions GetDimensions() const = 0;
	virtual bool IsLoaded() const = 0;
	virtual void CopyImageData(std::vector<uint8_t>& data) = 0;
  virtual Image::Pixel GetPixel(std::size_t x, std::size_t y) const = 0;
  virtual void SetPixel(std::size_t x, std::size_t y, const Image::Pixel& p) = 0;
};
}  // namespace pagoda::image::core
