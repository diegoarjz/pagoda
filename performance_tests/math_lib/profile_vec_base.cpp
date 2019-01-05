#define STATISTICS_ENABLED

#include <common/profiler.h>
#include <common/statistics.h>
#include <common/version.h>
#include <math_lib/vec_base.h>

#include "../performance_test.h"

using namespace selector;

template<int Size, class Rep, int Repetitions>
void single_value_constructor()
{
	for (auto i = 0u; i < Repetitions; ++i)
	{
		VecBase<Size, Rep> v(0);
	}
}

template<class Rep, int Repetitions>
void elements_2_constructor()
{
	for (auto i = 0u; i < Repetitions; ++i)
	{
		VecBase<2, Rep> v2(0, 0);
	}
}

template<class Rep, int Repetitions>
void elements_3_constructor()
{
	for (auto i = 0u; i < Repetitions; ++i)
	{
		VecBase<3, Rep> v3(0, 0, 0);
	}
}

template<class Rep, int Repetitions>
void elements_4_constructor()
{
	for (auto i = 0u; i < Repetitions; ++i)
	{
		VecBase<4, Rep> v4(0, 0, 0, 0);
	}
}

template<int Size, class Rep, int Repetitions>
void copy_constructor()
{
	VecBase<Size, Rep> v0(0);
	for (auto i = 0u; i < Repetitions; ++i)
	{
		VecBase<Size, Rep> v(v0);
	}
}

template<int Size, class Rep, int Repetitions>
void assignment_operator()
{
	VecBase<Size, Rep> v0(0);
	VecBase<Size, Rep> v1(0);
	for (auto i = 0u; i < Repetitions; ++i)
	{
		v1 = v0;
	}
}

template<int Size, class Rep, int Repetitions>
void comparions()
{
	VecBase<Size, Rep> v0(0);
	VecBase<Size, Rep> v1(0);
	bool result = false;
	for (auto i = 0u; i < Repetitions; ++i)
	{
		result = v1 == v0;
		result = v1 != v0;
	}
}

template<int Size, class Rep, int Repetitions>
void conversion_operator()
{
	VecBase<Size, Rep> v0(0);
	Rep *elements;

	for (auto i = 0u; i < Repetitions; ++i)
	{
		elements = static_cast<Rep *>(v0);
	}
}

std::string PerformanceTest::TestName() { return __FILE__; }

void PerformanceTest::Run(int argc, char *argv[])
{
	std::cout << "Running " << __FILE__ << std::endl;
	std::cout << "Selector Version: " << selector::get_version_string() << std::endl;
	std::cout << "Selector Build Number: " << selector::get_build_number() << std::endl;
	std::cout << "Selector Build Date: " << selector::get_build_date() << std::endl;
	std::cout << std::endl;

	// Vec 2 Float
	{
		OneShotProfiler p("Vec2 Float Elements Constructor");
		elements_2_constructor<float, 100000>();
	}
	{
		OneShotProfiler p("Vec2 Float Single Value Constructor");
		single_value_constructor<2, float, 100000>();
	}
	{
		OneShotProfiler p("Vec2 Float Copy Constructor");
		copy_constructor<2, float, 100000>();
	}
	{
		OneShotProfiler p("Vec2 Float Assignment Operation");
		assignment_operator<2, float, 100000>();
	}
	{
		OneShotProfiler p("Vec2 Float Comparisons");
		comparions<2, float, 100000>();
	}
	{
		OneShotProfiler p("Vec2 Float Conversion");
		conversion_operator<2, float, 100000>();
	}

	// Vec 3 Float
	{
		OneShotProfiler p("Vec3 Float Elements Constructor");
		elements_3_constructor<float, 100000>();
	}
	{
		OneShotProfiler p("Vec3 Float Single Value Constructor");
		single_value_constructor<3, float, 100000>();
	}
	{
		OneShotProfiler p("Vec3 Float Copy Constructor");
		copy_constructor<3, float, 100000>();
	}
	{
		OneShotProfiler p("Vec3 Float Assignment Operation");
		assignment_operator<3, float, 100000>();
	}
	{
		OneShotProfiler p("Vec3 Float Comparisons");
		comparions<3, float, 100000>();
	}
	{
		OneShotProfiler p("Vec3 Float Conversion");
		conversion_operator<3, float, 100000>();
	}

	// Vec 4 Float
	{
		OneShotProfiler p("Vec4 Float Elements Constructor");
		elements_4_constructor<float, 100000>();
	}
	{
		OneShotProfiler p("Vec4 Float Single Value Constructor");
		single_value_constructor<4, float, 100000>();
	}
	{
		OneShotProfiler p("Vec4 Float Copy Constructor");
		copy_constructor<4, float, 100000>();
	}
	{
		OneShotProfiler p("Vec4 Float Assignment Operation");
		assignment_operator<4, float, 100000>();
	}
	{
		OneShotProfiler p("Vec4 Float Comparisons");
		comparions<4, float, 100000>();
	}
	{
		OneShotProfiler p("Vec4 Float Conversion");
		conversion_operator<4, float, 100000>();
	}

	// Vec 10 Float
	{
		OneShotProfiler p("Vec10 Float Single Value Constructor");
		single_value_constructor<10, float, 100000>();
	}
	{
		OneShotProfiler p("Vec10 Float Copy Constructor");
		copy_constructor<10, float, 100000>();
	}
	{
		OneShotProfiler p("Vec10 Float Assignment Operation");
		assignment_operator<10, float, 100000>();
	}
	{
		OneShotProfiler p("Vec10 Float Comparisons");
		comparions<10, float, 100000>();
	}
	{
		OneShotProfiler p("Vec10 Float Conversion");
		conversion_operator<10, float, 100000>();
	}
}
