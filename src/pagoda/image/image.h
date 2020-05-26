#pragma once

#include <memory>
#include <utility>

#include <boost/filesystem.hpp>

namespace pagoda::image
{
namespace core
{
class ImageImplementation;
class ImageImplementationFactory;
}  // namespace core

namespace io
{
class ImageReaderFactory;
}

class Image
{
public:
	using Dimensions = std::pair<std::size_t, std::size_t>;

	Image(const boost::filesystem::path& file);

	Dimensions GetDimensions() const;
	bool IsLoaded() const;
	void Load();

	void CopyImageData(std::vector<uint8_t>& data);

private:
	std::shared_ptr<core::ImageImplementation> m_implementation;

	static core::ImageImplementationFactory* GetImageImplementationFactory();
	static io::ImageReaderFactory* GetImageReaderFactory();

	static std::unique_ptr<core::ImageImplementationFactory> s_implementationFactory;
	static std::unique_ptr<io::ImageReaderFactory> s_imageReaderFactory;
};

using ImagePtr = std::shared_ptr<Image>;
}  // namespace pagoda::image
