#pragma once

#include <pagoda/common/exception/exception.h>

#include <string>

namespace pagoda::objects
{
class UnknownOperation : public common::exception::Exception
{
public:
	UnknownOperation(const std::string operationName);
};
}  // namespace pagoda::objects

