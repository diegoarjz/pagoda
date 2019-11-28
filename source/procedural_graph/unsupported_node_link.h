#pragma once

#include "common/exception.h"

namespace selector
{
class UnsupportedNodeLink : Exception
{
public:
	UnsupportedNodeLink(const std::string &direction, const std::string nodeType);
};
}  // namespace selector
