#include "file_util.h"

#include "exception.h"

#include <fstream>
#include <iostream>

namespace selector::file_util
{
std::string LoadFileToString(const boost::filesystem::path &path) { return LoadFileToString(path.string()); }

std::string LoadFileToString(const std::string &filePath)
{
	std::string str;
	std::ifstream file(filePath);
	if (!file)
	{
		throw Exception("Unable to open file '" + filePath + "' for reading.");
	}
	file.seekg(0, std::ios::end);
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return str;
}

void WriteStringToFile(const boost::filesystem::path &filePath, const std::string &contents)
{
	WriteStringToFile(filePath.string(), contents);
}

void WriteStringToFile(const std::string &filePath, const std::string &contents)
{
	std::ofstream file(filePath);
	if (!file)
	{
		throw Exception("Unable to open file '" + filePath + "' for writing.");
	}
	file << contents;
	file.close();
}

bool CreateDirectories(const std::string &path) { return CreateDirectories(boost::filesystem::path(path)); }
bool CreateDirectories(const boost::filesystem::path &path)
{
	try
	{
		return boost::filesystem::create_directories(path);
	}
	catch (...)
	{
		throw Exception("Unable to create directories");
	}
}
}  // namespace selector::file_util
