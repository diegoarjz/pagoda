#include "version.h"

#include <sstream>

namespace selector
{
std::string get_version_string() { return SELECTOR_VERSION; }

std::string get_build_number() { return SELECTOR_BUILD_NUMBER; }

std::string get_build_date() { return SELECTOR_BUILD_DATE; }

#if SELECTOR_INCLUDE_GIT_INFO
std::string get_git_branch() { return GIT_BRANCH; }

std::string get_git_commit_hash() { return GIT_COMMIT_HASH; }

std::string get_git_commit_author() { return GIT_COMMIT_AUTHOR; }

std::string get_git_commit_subject() { return GIT_COMMIT_SUBJECT; }
#endif

std::string get_version_information()
{
	std::stringstream ss;

	ss << "Selector version " << get_version_string();
	if (!get_build_number().empty())
	{
		ss << "\n Build Number: " << get_build_number();
	}
	if (!get_build_date().empty())
	{
		ss << "\n Build Date: " << get_build_date();
	}

#if SELECTOR_INCLUDE_GIT_INFO
	{
		ss << "\n\nGit:" << std::endl;
		ss << " Branch:         " << get_git_branch() << " (" << get_git_commit_hash() << ")" << std::endl;
		ss << " Commit Author:  " << get_git_commit_author() << std::endl;
		ss << " Commit Subject: " << get_git_commit_subject() << std::endl;
	}
#endif

	return ss.str();
}
}  // namespace selector
