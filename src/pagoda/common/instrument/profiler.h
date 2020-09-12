#ifndef PAGODA_COMMON_PROFILER_H_
#define PAGODA_COMMON_PROFILER_H_

#include "pagoda/common/utils.h"

#include <boost/chrono.hpp>

#include <cstdint>
#include <iostream>
#include <set>
#include <stack>

namespace pagoda::common::instrument
{
/**
 * Provides an entry point for the profiling system.
 * Guarantees that each thread has a different profile history.
 * Users of the profiler manager should not use this class directly, unless to gain access
 * to the recorded data.
 *
 * The ProfilerManager accepts nested calls to the EnterFunction which should always have
 * a corresponding call to ExitFunction. Time spent in a nested function doesn't contribute to
 * the outer function's time.
 */
class ProfilerManager
{
	public:
	/// Clock used to measure times
	using Clock = boost::chrono::thread_clock;
	/// The time point type used
	using TimePoint = Clock::time_point;
	/// The time counter type used
	using TimeCounter = Clock::duration;

	/**
	 * Contains data regarding a profiled function or chunk of code.
	 */
	struct ProfileEntry
	{
		ProfileEntry(const char* name, const char* file, const int line);

		const char* m_name;
		const char* m_file;
		const int m_line;
		std::int64_t m_calls;
		TimeCounter m_time;
		TimePoint m_lastTime;
	};  // struct ProfileEntry

	private:
	/**
	 * Sorts ProfilerEntries in the m_profileLog.
	 */
	struct ProfileSorter
	{
		bool operator()(const ProfileEntry& lhs, const ProfileEntry& rhs) const;
	};  // struct ProfileSorter

	public:
	using ProfileLog = std::set<ProfileEntry, ProfileSorter>;

	/**
	 * Returns a thread local instance of a ProfilerManger.
	 */
	static ProfilerManager* Instance();

	/**
	 * Indicates that the profiler entered a new function or code chunk that
	 * should be profiled.
	 */
	void EnterFunction(const char* name, const char* file, const int line);
	/**
	 * Indicates that the profiler exited a function or code chunk.
	 */
	void ExitFunction();
	/**
	 * Returns the logged data to be processed somewhere.
	 */
	inline const ProfileLog& GetLogData() const { return m_profileLog; }

	/**
	 * Clears all logged data.
	 */
	void ClearLogData()
	{
		m_profileLog = ProfileLog();
		m_profileStack = std::stack<ProfileEntry*>();
	}

	private:
	ProfilerManager();
	~ProfilerManager();

	/// Keeps a record of all logged data
	ProfileLog m_profileLog;
	/// Used to support nested logs
	std::stack<ProfileEntry*> m_profileStack;
};  // class ProfilerManager

/**
 * On construction, creates calls the ProfilerManager::EnterFunction and, on exit, calls
 * the ProfilerManager::ExitFunction. It is also possible to prematurely end the profiler, which can be used for named
 * profilers.
 */
class Profiler
{
	public:
	Profiler(const char* name, const char* file, const int line);
	~Profiler();

	void EndProfile();

	private:
	bool m_onStack;
};  // class Profiler

/**
 * Scoped one shot profiler that logs the time spent within a block to the
 * statistics.
 * Note that, unlike the other profiler, this isn't nested. Therefore, the time
 * spent within other nested function calls is also measured.
 */
class OneShotProfiler
{
	public:
	/// Clock used to measure times
	using Clock = boost::chrono::thread_clock;
	/// The time point type used
	using TimePoint = Clock::time_point;
	/// The time counter type used
	using TimeCounter = Clock::duration;

	/**
	 * Statistics group name.
	 * Defaults to OneShotProfilers.
	 * Must exist before using this profiler.
	 */
	static const char* kStatisticsName;

	explicit OneShotProfiler(const char* name);
	~OneShotProfiler();

	private:
	TimePoint m_start;
	const char* m_name;
};  // class OneShotProfiler

/**
 * Base class used for writing the logged data.
 * Processing the data shouldn't be done within subclasses but, instead,
 * the data should be written to a file to be processed by another tool.
 */
class ProfilerLogger
{
	/* TODO: These classes shouldn't really be called loggers... */
	public:
	ProfilerLogger(const ProfilerManager* manager);
	virtual ~ProfilerLogger();

	virtual void Log(std::size_t nLines = 0) = 0;

	protected:
	const ProfilerManager* m_manager;
};  // class ProfilerLogger

/**
 * Simple logger to write to the console.
 * Displays the results in decreasing order of time spent in function.
 */
class ConsoleProfilerLogger : public ProfilerLogger
{
	public:
	explicit ConsoleProfilerLogger(const ProfilerManager* manager);
	virtual ~ConsoleProfilerLogger();

	void Log(std::size_t nLines = 0) final;
};  // class ConsoleProfilerLogger

#ifdef PAGODA_PROFILER_ACTIVE

#define START_PROFILE ::pagoda::common::instrument::Profiler __PROFILER__(__FUNCTION__, __FILE__, __LINE__)
#define START_NAMED_PROFILE(x) ::pagoda::common::instrument::Profiler x(#x, __FILE__, __LINE__)

#define END_NAMED_PROFILE(x) (x).EndProfile()

#else

#define START_PROFILE
#define START_NAMED_PROFILE(x)
#define END_NAMED_PROFILE

#endif

}  // namespace pagoda::common::instrument

#endif
