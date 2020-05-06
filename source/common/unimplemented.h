#pragma once

#include "exception.h"

namespace pagoda
{
class Unimplemented : public Exception
{
public:
	Unimplemented(const char *file, const char *function, uint32_t line);
};

#define UNIMPLEMENTED throw pagoda::Unimplemented(__FILE__, __FUNCTION__, __LINE__);
}  // namespace pagoda
