#pragma once

#include "common/exception.h"

#include <string>

namespace selector
{
class UnknownNodeTypeException : public Exception
{
public:
	UnknownNodeTypeException(const std::string &nodeType);
};
}  // namespace selector
