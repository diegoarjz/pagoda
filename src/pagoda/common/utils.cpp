#include "utils.h"

#include <regex>

namespace pagoda::common
{
bool is_float(const std::string &str)
{
	static const std::regex float_regex("^[-+]?[0-9]*\\.?[0-9]+$");
	return std::regex_match(str, float_regex);
}

}  // namespace pagoda::common
