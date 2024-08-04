#include <iostream>
#include <ffinder/ffinder.h>

//#include <thread>
//#include <chrono>

int main(int argc, char** cmd_line)
{
	//std::this_thread::sleep_for(std::chrono::seconds(5));

	auto finder = ffinder::FilesDuplicateFinder::Instance(cmd_line[1]);
	if (!finder)
		return 1;

	std::cout << finder->GetOutput();

	return 0;
}