#pragma once

#include "image_implementation.h"

#include <pagoda/common/exception/exception.h>

#include "boost/gil/algorithm.hpp"
#include <boost/gil/extension/toolbox/metafunctions/get_pixel_type.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>

#include <memory>

namespace pagoda::image::core {
template <class ImageData>
class ImageImplementation : public ImageImplementationBase {
public:
  using ImageData_t = ImageData;
  using Point_t = typename ImageData_t::point_t;
  using Pixel_t =
      typename boost::gil::get_pixel_type<typename ImageData_t::view_t>::type;
  static constexpr std::size_t NumChannels = Point_t::num_dimensions;

  ImageImplementation(std::unique_ptr<ImageData> &&imageData)
      : m_imageData{std::move(imageData)} {}

  ImageImplementation() : m_imageData(nullptr) {}

  bool Reserve(const Image::Dimensions &dimensions) override {
    if (m_imageData == nullptr) {
      m_imageData =
          std::make_unique<ImageData_t>(Point_t(dimensions.x, dimensions.y));
    } else {
      m_imageData->recreate(dimensions.x, dimensions.y);
    }
    return true;
  }

  Image::Dimensions GetDimensions() const override {
    if (!IsLoaded()) {
      throw common::exception::Exception(
          "Trying to get RGB8Image dimensions but image not loaded");
    }
    auto dim = m_imageData->dimensions();
    return {static_cast<std::size_t>(dim.x), static_cast<std::size_t>(dim.y)};
  }

  bool IsLoaded() const override { return m_imageData != nullptr; }

  void CopyImageData(std::vector<uint8_t> &data) override {
    auto view = boost::gil::view(*m_imageData);
    const auto dims = GetDimensions();
    data.reserve(dims.x * dims.y * NumChannels);
    boost::gil::for_each_pixel(
        view, [](Pixel_t &p) { /*copyPixelValue(data, p);*/ });
  }

  Image::Pixel GetPixel(std::size_t x, std::size_t y) const override {
    //auto view = boost::gil::view(*m_imageData);
    Image::Pixel p{};
    //copyToPixel<NumChannels>(p, view(x, y));
    return p;
  }

  void SetPixel(std::size_t x, std::size_t y, const Image::Pixel &p) override {
    //auto view = boost::gil::view(*m_imageData);
    //copyFromPixel<NumChannels>(p, view(x, y));
  }

private:

  std::unique_ptr<ImageData> m_imageData;
};

using RGB8Image = ImageImplementation<boost::gil::rgb8_image_t>;
using RGBA8Image = ImageImplementation<boost::gil::rgba8_image_t>;
} // namespace pagoda::image::core
