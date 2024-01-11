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

	void SetFilePath(const std::filesystem::path& p) override;
	void Read(boost::gil::rgb8_image_t& img) override;
  std::shared_ptr<Image> Read(Image::Format f) override;

	private:
  std::filesystem::path m_filePath;
};
}  // namespace pagoda::image::io
