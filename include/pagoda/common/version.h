#ifndef PAGODA_COMMON_VERSION_H_
#define PAGODA_COMMON_VERSION_H_

#include "pagoda/api.h"

#include <string>

namespace pagoda::common
{
/**
 * Returns the current version for the project.
 */
PAGODA_API std::string get_version_string();

/**
 * Returns the current build number for the project.
 */
PAGODA_API std::string get_build_number();

/**
 * Returns the date this project was built.
 */
PAGODA_API std::string get_build_date();

/**
 * Returns the compiler id used to build the project.
 */
PAGODA_API std::string get_compiler_id();

/**
 * Returns the OS the project has been built for.
 */
PAGODA_API std::string get_os();

/**
 * Returns a formatted version information string.
 */
PAGODA_API std::string get_version_information();

/**
 * Checks whether pagoda was compiled with a given feature.
 */
PAGODA_API bool has_feature(const std::string &featureName);
}  // namespace pagoda::common

#endif
