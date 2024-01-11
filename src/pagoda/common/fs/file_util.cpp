#include "file_util.h"

#include "pagoda/common/debug/logger.h"
#include "pagoda/common/exception/exception.h"

#include <boost/dll.hpp>

#include <fstream>
#include <iostream>

namespace pagoda::common::fs {
boost::filesystem::path GetExecutablePath() {
  return boost::dll::program_location();
}

boost::filesystem::path GetExecutableDir() {
  return boost::dll::program_location().parent_path();
}
std::string LoadFileToString(const boost::filesystem::path &path) {
  std::string str;
  std::ifstream file(path.string());
  if (!file) {
    throw exception::Exception("Unable to open file '" + path.string() +
                               "' for reading.");
  }
  file.seekg(0, std::ios::end);
  str.reserve(file.tellg());
  file.seekg(0, std::ios::beg);
  str.assign((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());

  return str;
}

void WriteStringToFile(const boost::filesystem::path &path,
                       const std::string &contents) {
  std::ofstream file(path.string());
  if (!file) {
    throw exception::Exception("Unable to open file '" + path.string() +
                               "' for writing.");
  }
  file << contents;
  file.close();
}

bool CreateDirectories(const boost::filesystem::path &path) {
  if (path.empty()) {
    return true;
  }

  try {
    return boost::filesystem::create_directories(path);
  } catch (const boost::filesystem::filesystem_error &e) {
    LOG_ERROR("File System Error: " << e.what());
    throw exception::Exception(
        "File System Error. Unable to create directories.");
  } catch (...) {
    throw exception::Exception("Unable to create directories");
  }
}
} // namespace pagoda::common::fs
