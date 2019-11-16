#pragma once

#include "common/exception.h"

#include <string>

namespace selector
{
class UnknownOperation : public Exception
{
public:
	UnknownOperation(const std::string operationName);
};
}  // namespace selector

