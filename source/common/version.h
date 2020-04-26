#ifndef SELECTOR_COMMON_VERSION_H_
#define SELECTOR_COMMON_VERSION_H_

#include <string>

namespace selector
{
/**
 * Returns the current version for the project.
 */
std::string get_version_string();

/**
 * Returns the current build number for the project.
 */
std::string get_build_number();

/**
 * Returns the date this project was built.
 */
std::string get_build_date();

/**
 * Returns a formatted version information string.
 */
std::string get_version_information();

/**
 * Checks whether selector was compiled with a given feature.
 */
bool has_feature(const std::string &featureName);
}  // namespace selector

#endif
