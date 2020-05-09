#pragma once

#include "common/exception.h"

namespace pagoda
{
class UnsupportedNodeLink : Exception
{
public:
	UnsupportedNodeLink(const std::string &direction, const std::string nodeType);
};
}  // namespace pagoda
