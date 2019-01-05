#define STATISTICS_ENABLED

#include <common/profiler.h>
#include <common/statistics.h>
#include <common/version.h>
#include <math_lib/vec_base.h>

#include "../performance_test.h"

using namespace selector;

std::string PerformanceTest::TestName() { return __FILE__; }

void PerformanceTest::Run(int argc, char *argv[])
{
	// Vec 2 Float
	{
		OneShotProfiler p("Vec2 Float Addition");
		Vec2F lhs(1, 2);
		Vec2F rhs(3, 2);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs + rhs;
		}
	}

	{
		OneShotProfiler p("Vec2 Float Subtraction");
		Vec2F lhs(1, 2);
		Vec2F rhs(3, 2);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs - rhs;
		}
	}

	// Vec 3 Float
	{
		OneShotProfiler p("Vec3 Float Addition");
		Vec3F lhs(1, 2, 3);
		Vec3F rhs(3, 2, 1);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs + rhs;
		}
	}

	{
		OneShotProfiler p("Vec3 Float Subtraction");
		Vec3F lhs(1, 2, 3);
		Vec3F rhs(3, 2, 1);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs - rhs;
		}
	}

	// Vec 4 Float
	{
		OneShotProfiler p("Vec4 Float Addition");
		Vec4F lhs(1, 2, 3, 4);
		Vec4F rhs(3, 2, 1, 0);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs + rhs;
		}
	}

	{
		OneShotProfiler p("Vec4 Float Subtraction");
		Vec4F lhs(1, 2, 3, 4);
		Vec4F rhs(3, 2, 1, 0);

		for (auto i = 0u; i < 100000; ++i)
		{
			lhs - rhs;
		}
	}
}
