#include <gtest/gtest.h>

#include <common/logger.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    auto returnVal = RUN_ALL_TESTS();

    selector::Logger::Shutdown();

    return returnVal;
}
