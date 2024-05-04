#include <iostream>
#include "version.h"

#include <ip_parser.h>

int main(int, char**)
{
	std::cout << "Version: " << PROJECT_VERSION_PATCH << std::endl;

	return 0;
}