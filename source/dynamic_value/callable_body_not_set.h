#pragma once

#include "common/exception.h"

namespace selector
{
class CallableBodyNotSet : public Exception
{
public:
	CallableBodyNotSet(const std::string &dynamicValueName);
};
}  // namespace selector
