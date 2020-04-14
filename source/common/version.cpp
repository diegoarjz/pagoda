#include "version.h"

#include <sstream>
#include <vector>

namespace selector
{
struct Feature
{
	std::string m_name;
	bool m_enabled;

	explicit operator std::string() const
	{
		if (m_enabled)
		{
			return "+" + m_name;
		}
		return "-" + m_name;
	}
};

std::vector<Feature> get_features()
{
#define HAS_FEATURE(F, NAME)                      \
	if (std::string(F) == "ON")                   \
	{                                             \
		features.push_back(Feature{NAME, true});  \
	}                                             \
	else                                          \
	{                                             \
		features.push_back(Feature{NAME, false}); \
	}

	std::vector<Feature> features;
	HAS_FEATURE(SELECTOR_PROFILER_ACTIVE, "Profiler")
	HAS_FEATURE(SELECTOR_ENABLE_ASSERTIONS, "DebugAsserts")
	return features;
}

std::string get_version_string() { return SELECTOR_VERSION; }

std::string get_build_number() { return SELECTOR_BUILD_NUMBER; }

std::string get_build_date() { return SELECTOR_BUILD_DATE; }

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

	ss << "\n\nConditional features: ";
	for (const auto &f : get_features())
	{
		ss << static_cast<std::string>(f) << " ";
	}

#if SELECTOR_INCLUDE_GIT_INFO
	{
		ss << "\n\nGit:" << std::endl;
		ss << " Branch:         " << GIT_BRANCH << " (" << GIT_COMMIT_HASH << ")" << std::endl;
		ss << " Commit Author:  " << GIT_COMMIT_AUTHOR << std::endl;
		ss << " Commit Subject: " << GIT_COMMIT_SUBJECT << std::endl;
	}
#endif

	return ss.str();
}
}  // namespace selector
