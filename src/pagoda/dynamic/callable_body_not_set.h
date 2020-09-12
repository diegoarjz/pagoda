#pragma once

#include <pagoda/common/exception/exception.h>

namespace pagoda::dynamic
{
class CallableBodyNotSet : public common::exception::Exception
{
	public:
	CallableBodyNotSet(const std::string &dynamicValueName);
};
}  // namespace pagoda::dynamic
