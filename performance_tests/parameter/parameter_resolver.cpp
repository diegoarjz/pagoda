#define STATISTICS_ENABLED

#include <common/profiler.h>
#include <parameter/context.h>
#include <parameter/float_parameter.h>
#include <parameter/parameter.h>
#include <parameter/parameter_resolver.h>

#include "../performance_test.h"

#include <sstream>
#include <vector>

using namespace selector;

std::string PerformanceTest::TestName() { return __FILE__; }

std::vector<std::shared_ptr<Context>> CreateLinearContexts(int num)
{
	std::vector<std::shared_ptr<Context>> contexts;

	contexts.push_back(std::make_shared<Context>("ctx"));

	for (auto i = 0u; i < num - 1; ++i)
	{
		auto ctx = std::make_shared<Context>("ctx");
		ctx->SetParent(contexts.back());
		contexts.push_back(ctx);
	}

	return contexts;
}

void RunPathReference()
{
	int num = 1000;
	auto contexts = CreateLinearContexts(num);
	contexts.back()->CreateParameter<FloatParameter>("par", 1.0);

	std::stringstream ss;
	ss << "/";
	for (auto i = 0; i < num; ++i)
	{
		ss << "ctx/";
	}
	ss << "par";

	auto path = ss.str();

	ParameterResolver resolver(contexts.front());

	{
		OneShotProfiler p("ResolveStringPath");
		resolver.Resolve(path, contexts.front());
	}
}

void RunComponentsPath()
{
	int num = 1000;
	auto contexts = CreateLinearContexts(num);
	contexts.back()->CreateParameter<FloatParameter>("par", 1.0);

	std::stringstream ss;
	ss << "/";
	for (auto i = 0; i < num; ++i)
	{
		ss << "ctx/";
	}
	ss << "par";

	auto path = ss.str();
	ParameterPath par_path = ParameterPath::CreatePath(path);

	ParameterResolver resolver(contexts.front());

	{
		OneShotProfiler p("ResolveComponentPath");
		resolver.Resolve(par_path, contexts.front());
	}
}

void PerformanceTest::Run(int argc, char *argv[])
{
	if (argc < 2)
	{
		return;
	}

	if (strcmp(argv[1], "string_path") == 0)
	{
		RunPathReference();
	}
	else if (strcmp(argv[1], "components_path") == 0)
	{
		RunComponentsPath();
	}
}
