#include "profiler.h"

#include "assertions.h"
#include "statistics.h"

#include <iostream>
#include <list>

namespace selector
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

void ConsoleProfilerLogger::Log()
{
	using Entry = typename ProfilerManager::ProfileEntry;

	auto& profile_log = m_manager->GetLogData();
	std::list<Entry> sorted_by_time(std::begin(profile_log), std::end(profile_log));

	sorted_by_time.sort([](const Entry& lhs, const Entry& rhs) { return lhs.m_time > rhs.m_time; });

	for (auto e : sorted_by_time)
	{
		std::cout << e.m_name << "(" << e.m_file << ":" << e.m_line << ") - " << e.m_calls << " calls " << e.m_time
		          << std::endl;
	}
}

}  // namespace selector
