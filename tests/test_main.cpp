//
// Created by Diego S. Seabra on 15/11/22.
//

#include <CommandLineTestRunner.h>
#include <TestHarness.h>

TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
    CHECK_EQUAL(1, 1);
}

TEST(FirstTestGroup, SecondTest)
{
    STRCMP_EQUAL("hello", "hello");
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
