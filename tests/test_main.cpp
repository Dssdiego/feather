//
// Created by Diego S. Seabra on 15/11/22.
//

#include <CommandLineTestRunner.h>
#include <TestHarness.h>

#include "test_rect.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
