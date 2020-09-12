#ifndef PAGODA_COMMON_UTILS_H_
#define PAGODA_COMMON_UTILS_H_

#include <string>

namespace pagoda::common
{
/// Suppresses warnings about unused values and guarantees that the code is not
/// optimised out
#define UNUSED(x)      \
	do {                 \
		(void)sizeof((x)); \
	} while (0)

bool is_float(const std::string &str);

}  // namespace pagoda::common

#endif
