//1409
/*int main(int, char**)
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
}*/

//1877
/*int main(int, char**)
{
	int code1, code2;
	std::cin >> code1;
	std::cin >> code2;

	int thief_code = 0;

	while (thief_code <= 9999)
	{
		if (thief_code % 2 == 0)
		{
			if (thief_code == code1)
			{
				std::cout << "yes";
				return 0;
			}
		}
		else
		{
			if (thief_code == code2)
			{
				std::cout << "yes";
				return 0;
			}
		}

		thief_code += 1;
	}

	std::cout << "no";

	return 0;
}*/

//2001
/*int main(int, char**)
{
	std::pair<int, int> w1, w2, w3;
	std::cin >> w1.first;
	std::cin >> w1.second;
	std::cin >> w2.first;
	std::cin >> w2.second;
	std::cin >> w3.first;
	std::cin >> w3.second;

	int basket1berry1 = w1.first;
	int basket2berry2 = w1.second;

	int basket1berry1berry2 = w2.first;
	int basket2 = w2.second;

	int berry2 = basket2berry2 - basket2;

	int basket1 = w3.first;

	int berry1 = basket1berry1 - basket1;

	std::cout << berry1 << ' ' << berry2;

	return 0;
}*/



//1264
/*int main(int, char**)
{
	int N, M;
	std::cin >> N;
	std::cin >> M;

	std::cout << (N * (M + 1));

	return 0;
}*/

#include <iostream>

int main(int, char**)
{
	int cars_per_minute, time_in_minutes;
	std::cin >> cars_per_minute;
	std::cin >> time_in_minutes;

	int cars_per_minute_arr[3] = { 0 };
	std::cin >> cars_per_minute_arr[0];
	std::cin >> cars_per_minute_arr[1];
	std::cin >> cars_per_minute_arr[2];

	int cars_in_traffic_jam = 0;

	for (int cars : cars_per_minute_arr)
	{
		cars_in_traffic_jam += cars;

		if (cars_in_traffic_jam >= cars_per_minute)
		{
			cars_in_traffic_jam = cars_in_traffic_jam - cars_per_minute;
		}
		else
		{
			cars_in_traffic_jam = 0;
		}
	}

	std::cout << cars_in_traffic_jam;

	return 0;
}