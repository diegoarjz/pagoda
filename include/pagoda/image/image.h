#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <filesystem>

namespace pagoda::image {
namespace core {
class ImageImplementationBase;
class ImageImplementationFactory;
} // namespace core

namespace io {
class ImageReaderFactory;
}

class Image {
public:
  enum class Format { RGB8, RGBA8 };

  struct Dimensions {
    std::size_t x;
    std::size_t y;
  };

  struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    template <int C> uint8_t &at() { return *(&r + C); }
    template <int C> const uint8_t &at() const { return *(&r + C); }
  };

  Image(const Dimensions &d, Format format);
  Image(std::unique_ptr<core::ImageImplementationBase> &&image);

  Format GetFormat() const { return Format::RGB8; }

  Dimensions GetDimensions() const;
  bool IsLoaded() const;

  void CopyImageData(std::vector<uint8_t> &data);
  Pixel GetPixel(std::size_t x, std::size_t y) const;
  void SetPixel(std::size_t x, std::size_t y, const Pixel &p);

  static std::shared_ptr<Image>
  CreateFromFile(const std::filesystem::path &file, Format format);

private:
  std::shared_ptr<core::ImageImplementationBase> m_impl;

  static core::ImageImplementationFactory *GetImageImplementationFactory();
  static io::ImageReaderFactory *GetImageReaderFactory();

  static std::unique_ptr<core::ImageImplementationFactory>
      s_implementationFactory;
  static std::unique_ptr<io::ImageReaderFactory> s_imageReaderFactory;
};

using ImagePtr = std::shared_ptr<Image>;
} // namespace pagoda::image
