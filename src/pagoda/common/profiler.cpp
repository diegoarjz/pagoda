#include "profiler.h"

#include "assertions.h"
#include "statistics.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>

namespace pagoda
{
ProfilerManager::ProfilerManager() {}

ProfilerManager::~ProfilerManager()
{
	DBG_ASSERT_MSG(m_profileStack.size() == 0, "Profiler manager destroyed before all functions exited");
}

ProfilerManager* ProfilerManager::Instance()
{
	thread_local static ProfilerManager instance;

	return &instance;
}

void ProfilerManager::EnterFunction(const char* name, const char* file, const int line)
{
	// It is assured that we will not break the set ordering.
	// We won't modify the entry's name or line which are used for sorting.
	ProfileEntry* entry = const_cast<ProfileEntry*>(&(*(m_profileLog.insert(ProfileEntry(name, file, line)).first)));

	TimePoint now = Clock::now();
	entry->m_calls++;
	entry->m_lastTime = now;

	if (m_profileStack.size() > 0)
	{
		auto stackTop = m_profileStack.top();
		stackTop->m_time += now - stackTop->m_lastTime;
	}

	m_profileStack.push(entry);
}

void ProfilerManager::ExitFunction()
{
	DBG_ASSERT_MSG(m_profileStack.size() > 0,
	               "ExitFunction expects to be called when at least a ProfileEntry is in the stack");

	auto now = Clock::now();

	auto stackTop = m_profileStack.top();
	m_profileStack.pop();

	stackTop->m_time += now - stackTop->m_lastTime;

	if (m_profileStack.size() > 0)
	{
		stackTop = m_profileStack.top();
		stackTop->m_lastTime = now;
	}
}

ProfilerManager::ProfileEntry::ProfileEntry(const char* name, const char* file, const int line)
    : m_name(name), m_file(file), m_line(line), m_calls(0), m_time(0), m_lastTime()
{
}

bool ProfilerManager::ProfileSorter::operator()(const ProfileEntry& lhs, const ProfileEntry& rhs) const
{
	if (lhs.m_line == rhs.m_line)
	{
		return lhs.m_name > rhs.m_name;
	}

	return lhs.m_line > rhs.m_line;
}

Profiler::Profiler(const char* name, const char* file, const int line) : m_onStack(true)
{
	ProfilerManager::Instance()->EnterFunction(name, file, line);
}

Profiler::~Profiler() { EndProfile(); }

void Profiler::EndProfile()
{
	if (m_onStack)
	{
		ProfilerManager::Instance()->ExitFunction();
		m_onStack = false;
	}
}

const char* OneShotProfiler::kStatisticsName = "OneShotProfilers";
OneShotProfiler::OneShotProfiler(const char* name) : m_start(Clock::now()), m_name(name) {}

OneShotProfiler::~OneShotProfiler()
{
	auto now = Clock::now();
	OneShotProfilerStats::Instance()
	    ->AddContainer<typename StatisticsManager::AccumulatorContainer<TimeCounter>>(m_name)
	    ->Log(now - m_start);
}

ProfilerLogger::ProfilerLogger(const ProfilerManager* manager) : m_manager(manager) {}

ProfilerLogger::~ProfilerLogger() {}

ConsoleProfilerLogger::ConsoleProfilerLogger(const ProfilerManager* manager) : ProfilerLogger(manager) {}

ConsoleProfilerLogger::~ConsoleProfilerLogger() {}

void ConsoleProfilerLogger::Log(std::size_t nLines)
{
	using Entry = typename ProfilerManager::ProfileEntry;

	auto& profile_log = m_manager->GetLogData();
	std::list<Entry> sorted_by_time(std::begin(profile_log), std::end(profile_log));

	sorted_by_time.sort([](const Entry& lhs, const Entry& rhs) { return lhs.m_time > rhs.m_time; });

	std::array<std::size_t, 5> columnsMaxSize;
	for (auto i = 0u; i < 5; ++i)
	{
		columnsMaxSize[i] = 0;
	}

	std::size_t count = 0;
	for (const auto& e : sorted_by_time)
	{
		columnsMaxSize[0] = std::max(columnsMaxSize[0], std::strlen(e.m_file));
		columnsMaxSize[1] = std::max(columnsMaxSize[1], std::to_string(e.m_line).size());
		columnsMaxSize[2] = std::max(columnsMaxSize[2], std::strlen(e.m_name));
		columnsMaxSize[3] = std::max(columnsMaxSize[3], std::to_string(e.m_calls).size());

		++count;
		if (nLines != 0 && count > nLines)
		{
			break;
		}
	}

	count = 0;
	for (const auto& e : sorted_by_time)
	{
		std::cout << std::setw(columnsMaxSize[0]) << e.m_file;
		std::cout << "(" << std::setw(columnsMaxSize[1]) << e.m_line << "): ";
		std::cout << std::setw(columnsMaxSize[2]) << e.m_name << " ";
		std::cout << std::setw(columnsMaxSize[3]) << e.m_calls << " ";
		std::cout << e.m_time << std::endl;
		++count;
		if (nLines != 0 && count > nLines)
		{
			break;
		}
	}
}

}  // namespace pagoda
