#ifndef SELECTOR_COMMON_FILE_UTIL_H_
#define SELECTOR_COMMON_FILE_UTIL_H_

#include <boost/filesystem.hpp>

#include <string>

namespace selector::file_util
{
std::string LoadFileToString(const boost::filesystem::path &path);
std::string LoadFileToString(const std::string &filePath);

void WriteStringToFile(const boost::filesystem::path &filePath, const std::string &contents);
void WriteStringToFile(const std::string &filePath, const std::string &contents);

bool CreateDirectories(const std::string &path);
bool CreateDirectories(const boost::filesystem::path &path);
}  // namespace selector::file_util

#endif
