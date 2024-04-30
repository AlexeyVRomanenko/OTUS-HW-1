#include "lib.h"
#include <iostream>

void lib::print_hello_world()
{
    std::cout << "Hello, world!" << std::endl;
}

int lib::calc_sum(int a, int b)
{
    return a + b;
}