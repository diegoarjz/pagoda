#pragma once

#include <pagoda/common/exception/exception.h>

namespace pagoda::graph
{
class UnsupportedNodeLink : common::exception::Exception
{
	public:
	UnsupportedNodeLink(const std::string &direction, const std::string nodeType);
};
}  // namespace pagoda::graph
