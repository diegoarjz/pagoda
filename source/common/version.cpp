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
	std::vector<Feature> features = {
#ifdef SELECTOR_PROFILER_ACTIVE
	    {"Profiler", true},
#else
	    {"Profiler", false},
#endif
#ifdef SELECTOR_ENABLE_ASSERTIONS
	    {"DebugAsserts", true},
#else
	    {"DebugAsserts", false},
#endif
	};
	return features;
}

bool has_feature(const std::string &featureName)
{
	for (const auto &f : get_features())
	{
		if (f.m_name == featureName)
		{
			return f.m_enabled;
		}
	}
	return false;
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
