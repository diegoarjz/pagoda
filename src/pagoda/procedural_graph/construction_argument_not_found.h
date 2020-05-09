#pragma once

#include "common/exception.h"

#include <string>

namespace pagoda
{
class ConstructionArgumentNotFound : public Exception
{
public:
	ConstructionArgumentNotFound(const std::string nodeName, const uint32_t nodeId, const std::string &argName);
};
}  // namespace pagoda
