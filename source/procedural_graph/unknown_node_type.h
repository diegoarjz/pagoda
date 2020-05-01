#pragma once

#include "common/exception.h"

#include <string>

namespace pagoda
{
class UnknownNodeTypeException : public Exception
{
public:
	UnknownNodeTypeException(const std::string &nodeType);
};
}  // namespace pagoda
