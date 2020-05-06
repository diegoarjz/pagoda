#include <gtest/gtest.h>

#include <common/logger.h>

#include "test_utils.h"

int main(int argc, char *argv[])
{
	bool shouldWriteFiles = false;
	for (auto i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]) == "--writeFiles")
		{
			shouldWriteFiles = true;
			break;
		}
	}

	PagodaTestFixtureBase::SetExecutablePath(argv[0]);
	PagodaTestFixtureBase::SetShouldWriteFiles(shouldWriteFiles);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	pagoda::Logger::Shutdown();

	return returnVal;
}
