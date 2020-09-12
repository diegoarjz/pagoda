#include "rgb8_image.h"
#include "boost/gil/algorithm.hpp"

#include <pagoda/image/io/image_reader.h>

#include <pagoda/common/exception/exception.h>

#include <memory>

using namespace pagoda::image::io;

using namespace boost;

namespace pagoda::image::core
{
RGB8Image::RGB8Image() : m_imageData(nullptr), m_reader(nullptr) {}

RGB8Image::~RGB8Image() {}

ImageImplementation::Dimensions RGB8Image::GetDimensions() const
{
	if (!IsLoaded()) {
		throw common::exception::Exception("Trying to get RGB8Image dimensions but image not loaded");
	}
	auto dim = m_imageData->dimensions();
	return {dim.x, dim.y};
}

bool RGB8Image::IsLoaded() const { return m_imageData != nullptr; }

void RGB8Image::Load()
{
	if (IsLoaded()) {
		return;
	}

	if (m_reader == nullptr) {
		throw common::exception::Exception("Trying to load an RGB8Image but reader not set.");
	}

	m_imageData = std::make_unique<boost::gil::rgb8_image_t>();
	m_reader->Read(*m_imageData);
}

void RGB8Image::CopyImageData(std::vector<uint8_t>& data)
{
	auto view = gil::view(*m_imageData);
	gil::for_each_pixel(view, [&data](gil::rgb8_pixel_t p) {
		data.push_back(gil::at_c<0>(p));
		data.push_back(gil::at_c<1>(p));
		data.push_back(gil::at_c<2>(p));
	});
}

void RGB8Image::SetImageReader(std::shared_ptr<ImageReader> reader) { m_reader = reader; }
}  // namespace pagoda::image::core
