#ifndef PAGODA_COMMON_FILE_UTIL_H_
#define PAGODA_COMMON_FILE_UTIL_H_

#include <boost/filesystem.hpp>

#include <string>

namespace pagoda::common::fs
{
std::string LoadFileToString(const boost::filesystem::path &path);

void WriteStringToFile(const boost::filesystem::path &filePath, const std::string &contents);

bool CreateDirectories(const boost::filesystem::path &path);

template<class T>
void GetAllFilesWithExtension(const boost::filesystem::path &dirPath, const std::string &ext, T inserter)
{
	if (!boost::filesystem::exists(dirPath) || !boost::filesystem::is_directory(dirPath)) {
		return;
	}

	boost::filesystem::directory_iterator iterator(dirPath);
	boost::filesystem::directory_iterator end;

	while (iterator != end) {
		if (boost::filesystem::is_regular_file(*iterator) && iterator->path().extension() == ext) {
			inserter = iterator->path().filename();
		}
		++iterator;
	}
}
}  // namespace pagoda::common::fs

#endif
