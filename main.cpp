#include <iostream>

int main(int, char**)
{
	int garry = 0, larry = 0;
	std::cin >> garry;
	std::cin >> larry;

	int total = garry + larry;
	if (total > 10)
		total = 10;
	else
		total = total - 1;

	std::cout << (total - garry) << " " << (total - larry);

	return 0;
}