#include <iostream>

#include "version.h"
#include <lib.h>

int main (int, char **) {
    std::cout << "Version: " << PROJECT_VERSION_PATCH << std::endl;
    lib::print_hello_world();
    return 0;
}
