#include "statistics.h"

namespace selector
{
StatisticsManager *StatisticsManager::Instance()
{
	static StatisticsManager sInstance;

	return &sInstance;
}

void StatisticsManager::AddGroup(StatsGroup *group)
{
	auto inserted = m_statsGroup.emplace(group->Name(), group);

	DBG_ASSERT_MSG(inserted.second, "StatsGroup with the same name already exists");
}

void StatisticsManager::Write(std::ostream &outStream)
{
	for (auto &group : m_statsGroup)
	{
		group.second->Write(outStream);
	}
}

OneShotProfilerStats *OneShotProfilerStats::Instance()
{
	static OneShotProfilerStats sInstance;
	return &sInstance;
}

void OneShotProfilerStats::Write(std::ostream &outStream)
{
	outStream << "\"" << Name() << "\":" << std::endl;

	for (auto &stat : m_stats)
	{
		outStream << "    \"" << stat.first << "\" : ";
		stat.second->Write(outStream);
		outStream << std::endl;
	}
}

}  // namespace selector
