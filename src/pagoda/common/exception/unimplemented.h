#pragma once

#include "pagoda/common/exception/exception.h"

namespace pagoda::common::exception
{
class Unimplemented : public Exception
{
public:
	Unimplemented(const char *file, const char *function, uint32_t line);
};

#define UNIMPLEMENTED throw pagoda::common::exception::Unimplemented(__FILE__, __FUNCTION__, __LINE__);
}  // namespace pagoda::common::exception
