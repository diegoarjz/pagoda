#include "performance_test.h"

#include <common/profiler.h>
#include <common/statistics.h>
#include <common/version.h>

#include <fstream>
#include <iostream>

using namespace selector;

void PerformanceTest::PrintVersion()
{
	std::cout << "Running " << TestName() << std::endl;
	std::cout << "Selector Version: " << selector::get_version_string() << std::endl;
	std::cout << "Selector Build Number: " << selector::get_build_number() << std::endl;
	std::cout << "Selector Build Date: " << selector::get_build_date() << std::endl;
	std::cout << std::endl;
}

void PerformanceTest::WriteStatistics()
{
	std::ofstream outFile("stats.yaml");
	StatisticsManager::Instance()->Write(outFile);
	outFile.close();

#ifdef SELECTOR_PROFILER_ACTIVE
	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log();
#endif
}

int main(int argc, char *argv[])
{
	PerformanceTest test;

	test.PrintVersion();

	test.Run(argc, argv);

	test.WriteStatistics();

	return 0;
}
