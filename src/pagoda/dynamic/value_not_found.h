#pragma once

#include <pagoda/common/exception/exception.h>

#include <stdexcept>
#include <string>

namespace pagoda::dynamic
{
class ValueNotFoundException : public common::exception::Exception
{
public:
	ValueNotFoundException(const std::string &name);
};
}  // namespace pagoda::dynamic
