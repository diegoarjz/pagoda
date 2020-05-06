#pragma once

#include "common/exception.h"

namespace pagoda
{
class CallableBodyNotSet : public Exception
{
public:
	CallableBodyNotSet(const std::string &dynamicValueName);
};
}  // namespace pagoda
