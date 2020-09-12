#include "image.h"

#include "image_type.h"

#include <pagoda/image/core/image_implementation_factory.h>
#include <pagoda/image/io/image_reader_factory.h>

#include <memory>

using namespace boost;

namespace pagoda::image
{
using namespace core;
using namespace io;

std::unique_ptr<ImageImplementationFactory> Image::s_implementationFactory;
std::unique_ptr<ImageReaderFactory> Image::s_imageReaderFactory;

Image::Image(const filesystem::path& file) : m_implementation(GetImageImplementationFactory()->Create(ImageType::RGB8))
{
	m_implementation->SetImageReader(GetImageReaderFactory()->CreateFromFilePath(file));
}

Image::Dimensions Image::GetDimensions() const { return m_implementation->GetDimensions(); }
bool Image::IsLoaded() const { return m_implementation->IsLoaded(); }
void Image::Load() { m_implementation->Load(); }

void Image::CopyImageData(std::vector<uint8_t>& data) { m_implementation->CopyImageData(data); }

ImageImplementationFactory* Image::GetImageImplementationFactory()
{
	if (s_implementationFactory == nullptr) {
		s_implementationFactory = std::make_unique<ImageImplementationFactory>();
	}

	return s_implementationFactory.get();
}

ImageReaderFactory* Image::GetImageReaderFactory()
{
	//
	if (s_imageReaderFactory == nullptr) {
		s_imageReaderFactory = std::make_unique<ImageReaderFactory>();
	}

	return s_imageReaderFactory.get();
}

}  // namespace pagoda::image
