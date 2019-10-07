#include "file_util.h"

#include <fstream>
#include <iostream>

namespace selector
{
std::string FileUtil::LoadFileToString(const std::string &filePath)
{
	std::string str;
	std::ifstream file(filePath);
	file.seekg(0, std::ios::end);
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return str;
}

void FileUtil::WriteStringToFile(const std::string &filePath, const std::string &contents)
{
	std::ofstream file(filePath);
	file << contents;
	file.close();
}

}  // namespace selector
