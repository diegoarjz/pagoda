#ifndef SELECTOR_COMMON_UTILS_H_
#define SELECTOR_COMMON_UTILS_H_

#include <string>

namespace selector
{
/// Suppresses warnings about unused values and guarantees that the code is not
/// optimised out
#define UNUSED(x)          \
	do                     \
	{                      \
		(void)sizeof((x)); \
	} while (0)

bool is_float(const std::string &str);

}  // namespace selector

#endif
