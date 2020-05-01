#pragma once

#include "common/exception.h"

#include <stdexcept>
#include <string>

namespace pagoda
{
class ValueNotFoundException : public Exception
{
public:
	ValueNotFoundException(const std::string &name);
};
}  // namespace pagoda
