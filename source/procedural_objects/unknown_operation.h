#pragma once

#include "common/exception.h"

#include <string>

namespace pagoda
{
class UnknownOperation : public Exception
{
public:
	UnknownOperation(const std::string operationName);
};
}  // namespace pagoda

