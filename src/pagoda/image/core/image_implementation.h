#pragma once

#include <memory>
#include <utility>
#include <vector>

namespace pagoda::image::io
{
class ImageReader;
}

namespace pagoda::image::core
{
class ImageImplementation
{
	public:
	using Dimensions = std::pair<std::size_t, std::size_t>;

	virtual ~ImageImplementation() {}

	virtual Dimensions GetDimensions() const = 0;
	virtual bool IsLoaded() const = 0;
	virtual void Load() = 0;
	virtual void CopyImageData(std::vector<uint8_t>& data) = 0;
	virtual void SetImageReader(std::shared_ptr<io::ImageReader> reader) = 0;
};
}  // namespace pagoda::image::core
