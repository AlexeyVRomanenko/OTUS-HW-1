#include <chrono>
#include <iostream>
#include <thread>
#include <cassert>
#include <threads.h>
#include <filesystem>
#include <ffinder/ffinder.h>

using namespace std::chrono_literals;

namespace fs = std::filesystem;

/*

--dir=


*/

int main(int argc, char** cmd_line)
{
	std::this_thread::sleep_for(3s);


	auto finder = ffinder::FilesDuplicateFinder::Instance(cmd_line[1]);
	if (!finder)
		return 1;

	return 0;
}