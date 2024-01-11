#include "image_reader_factory.h"

#include "png_reader.h"

#include <memory>

using namespace pagoda::common;

namespace pagoda::image::io
{
ImageReaderFactory::ImageReaderFactory() : Factory<ImageReader>("ImageReader")
{
	Register("png", []() { return std::make_shared<PngReader>(); });
}

ImageReaderFactory::~ImageReaderFactory() {}

std::shared_ptr<ImageReader> ImageReaderFactory::CreateFromFilePath(const std::filesystem::path &p)
{
	auto reader = Create(p.extension().string().substr(1));
	if (reader != nullptr) {
		reader->SetFilePath(p);
	}
	return reader;
}
}  // namespace pagoda::image::io
