#ifndef SELECTOR_COMMON_FILE_UTIL_H_
#define SELECTOR_COMMON_FILE_UTIL_H_

#include <string>

namespace selector
{
class FileUtil
{
public:
	static std::string LoadFileToString(const std::string &filePath);

    static void WriteStringToFile(const std::string &filePath, const std::string &contents);
};
}  // namespace selector

#endif
