#pragma once

#include "image_implementation.h"

#include <boost/gil/typedefs.hpp>

#include <memory>

namespace pagoda::image::core
{
class RGB8Image : public ImageImplementation
{
public:
	RGB8Image();
	virtual ~RGB8Image();

	Dimensions GetDimensions() const override;
	bool IsLoaded() const override;
	void Load() override;
	void CopyImageData(std::vector<uint8_t>& data) override;

	void SetImageReader(std::shared_ptr<io::ImageReader> reader) override;

private:
	std::unique_ptr<boost::gil::rgb8_image_t> m_imageData;
	std::shared_ptr<io::ImageReader> m_reader;
};
}  // namespace pagoda::image::core
