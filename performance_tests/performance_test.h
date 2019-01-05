#ifndef SELECTOR_PERFORMANCE_TESTS_H_
#define SELECTOR_PERFORMANCE_TESTS_H_

#include <string>

class PerformanceTest
{
public:
    void PrintVersion();
    std::string TestName();
    void Run(int argc, char *argv[]);
    void WriteStatistics();
}; // class PerformanceTest

#endif
