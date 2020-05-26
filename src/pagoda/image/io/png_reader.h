#pragma once

#include "image_reader.h"

#include <boost/filesystem.hpp>

namespace pagoda::image::io
{
class PngReader : public ImageReader
{
public:
	PngReader();
	virtual ~PngReader();

	void SetFilePath(const boost::filesystem::path& p) override;
	void Read(boost::gil::rgb8_image_t& img) override;

private:
	boost::filesystem::path m_filePath;
};
}  // namespace pagoda::image::io
