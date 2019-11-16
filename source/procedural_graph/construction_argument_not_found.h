#pragma once

#include "common/exception.h"

#include <string>

namespace selector
{
class ConstructionArgumentNotFound : public Exception
{
public:
	ConstructionArgumentNotFound(const std::string nodeName, const uint32_t nodeId, const std::string &argName);
};
}  // namespace selector
