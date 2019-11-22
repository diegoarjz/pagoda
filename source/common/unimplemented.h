#pragma once

#include "exception.h"

namespace selector
{
class Unimplemented : public Exception
{
public:
	Unimplemented(const char *file, const char *function, uint32_t line);
};

#define UNIMPLEMENTED throw selector::Unimplemented(__FILE__, __FUNCTION__, __LINE__);
}  // namespace selector
