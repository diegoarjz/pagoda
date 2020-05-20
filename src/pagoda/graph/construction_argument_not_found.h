#pragma once

#include "pagoda/common/exception/exception.h"

#include <string>

namespace pagoda::graph
{
class ConstructionArgumentNotFound : public common::exception::Exception
{
public:
	ConstructionArgumentNotFound(const std::string nodeName, const uint32_t nodeId, const std::string &argName);
};
}  // namespace pagoda::graph
