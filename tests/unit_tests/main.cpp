#include <gtest/gtest.h>

#include <common/logger.h>

#include "test_utils.h"

int main(int argc, char *argv[])
{
    SelectorTestFixtureBase::SetExecutablePath(argv[0]);
    SelectorTestFixtureBase::SetShouldWriteFiles(false);
    ::testing::InitGoogleTest(&argc, argv);

    auto returnVal = RUN_ALL_TESTS();

    selector::Logger::Shutdown();

    return returnVal;
}
