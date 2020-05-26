#include "png_reader.h"

#include <pagoda/common/debug/logger.h>

#include <boost/gil/extension/io/png/old.hpp>
#include <boost/gil/extension/io/png/read.hpp>
#include <boost/gil/io/read_and_convert_view.hpp>
#include <boost/gil/io/read_image.hpp>
#include <boost/gil/typedefs.hpp>

using namespace boost;

namespace pagoda::image::io
{
PngReader::PngReader() {}
PngReader::~PngReader() {}

void PngReader::SetFilePath(const boost::filesystem::path& p) { m_filePath = p; }
void PngReader::Read(boost::gil::rgb8_image_t& img) { gil::png_read_and_convert_image(m_filePath.string(), img); }
}  // namespace pagoda::image::io
