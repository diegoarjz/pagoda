#include <pagoda/common/instrument/profiler.h>

#include <gtest/gtest.h>

/*
 * In this unit test file we are using std::chrono instead of
 * boost::chrono because we always favour the std over other libraries
 * and, in this case, we want to use std::thread::sleep_for which takes
 * a std::duration object.
 */
#include <chrono>
#include <thread>

#include <iostream>

using namespace pagoda;
using namespace pagoda::common::instrument;

/*
void GetInstance(ProfilerManager** instance) { *instance = ProfilerManager::Instance(); }
TEST(Profiler, multi_threaded_should_return_different_instances)
{
    ProfilerManager* i1;
    ProfilerManager* i2;
    ProfilerManager* i0 = ProfilerManager::Instance();

    std::thread t1(GetInstance, &i1);
    std::thread t2(GetInstance, &i2);

    t1.join();
    t2.join();

    EXPECT_NE(i1, i2) << "ProfilerManager instances should be different for each thread";
    EXPECT_NE(i0, i1) << "ProfilerManager instances should be different for each thread";
    EXPECT_NE(i0, i2) << "ProfilerManager instances should be different for each thread";
}
*/

TEST(Profiler, single_call_should_log_one_entry)
{
	auto manager = ProfilerManager::Instance();
	manager->ClearLogData();
	const char* name = "test1";
	const char* file = "file1";
	const int line = 1;

	{
		Profiler p(name, file, line);

		ASSERT_EQ(manager->GetLogData().size(), 1);

		auto entry = *manager->GetLogData().begin();
		EXPECT_EQ(entry.m_name, name);
		EXPECT_EQ(entry.m_file, file);
		EXPECT_EQ(entry.m_line, line);
		EXPECT_EQ(entry.m_calls, 1);
		EXPECT_EQ(entry.m_time, ProfilerManager::TimeCounter(0));
	}

	EXPECT_EQ(manager->GetLogData().size(), 1);
}

TEST(Profiler, multiple_calls_should_increment_count)
{
	auto manager = ProfilerManager::Instance();
	manager->ClearLogData();
	const char* name = "test2";
	const char* file = "file2";
	const int line = 2;

	for (uint32_t i = 0; i < 2; ++i)
	{
		Profiler p(name, file, line);
		auto entry = *manager->GetLogData().begin();
		EXPECT_EQ(entry.m_calls, i + 1);
	}
}

TEST(Profiler, multiple_calls_should_not_create_new_entries)
{
	auto manager = ProfilerManager::Instance();
	manager->ClearLogData();
	const char* name = "test2";
	const char* file = "file2";
	const int line = 2;

	for (uint32_t i = 0; i < 2; ++i)
	{
		Profiler p(name, file, line);
	}

	ASSERT_EQ(manager->GetLogData().size(), 1);
}

TEST(Profiler, recursive_calls_should_increase_count)
{
	auto manager = ProfilerManager::Instance();
	manager->ClearLogData();
	const char* name1 = "test1";
	const char* file1 = "file1";
	const int line1 = 1;

	{
		Profiler p1(name1, file1, line1);
		{
			Profiler p2(name1, file1, line1);
		}
	}
	auto entry = *manager->GetLogData().begin();
	EXPECT_EQ(entry.m_calls, 2);
}

TEST(Profiler, recursive_calls_should_not_create_multiple_entries)
{
	auto manager = ProfilerManager::Instance();
	manager->ClearLogData();
	const char* name1 = "test1";
	const char* file1 = "file1";
	const int line1 = 1;

	{
		Profiler p1(name1, file1, line1);
		{
			Profiler p2(name1, file1, line1);
		}
	}
	ASSERT_EQ(manager->GetLogData().size(), 1);
}
