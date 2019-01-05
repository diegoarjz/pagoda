#include "version.h"

namespace selector
{
std::string get_version_string() { return SELECTOR_VERSION; }

std::string get_build_number() { return SELECTOR_BUILD_NUMBER; }

std::string get_build_date() { return SELECTOR_BUILD_DATE; }

}  // namespace selector
