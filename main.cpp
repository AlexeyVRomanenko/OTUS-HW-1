#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <execution>
#include <boost/filesystem.hpp>

#include <ip_parser.h>

namespace fs = boost::filesystem;

int main(int, char**)
{
	std::vector<std::string> lines;
	{
		const std::string file = fs::current_path().string() + R"(/ip_filter.tsv)";

		std::ifstream fstrm(file, std::ios::binary);
		if (!fstrm)
			return 1;

		std::string line;
		for (std::string line; std::getline(fstrm, line);)
			lines.push_back(line);
	}

	if (lines.empty())
		return 1;

	std::vector<ip_t> ips_parsed;
	ips_parsed.resize(lines.size());
	for (size_t i = 0, isz = lines.size(); i < isz; ++i) //better parallel
	{
		if (!ip_parser::parse(lines[i], ips_parsed[i]))
			return 1;
	}

	std::vector<ip_t> ips_sorted = ips_parsed;
	std::sort(ips_sorted.begin(), ips_sorted.end());

	std::vector<ip_t> parsed_ips_reversed(ips_sorted.size());
	std::reverse_copy(ips_sorted.begin(), ips_sorted.end(), parsed_ips_reversed.begin());

	//1. Полный список адресов после сортировки. Одна строка - один адрес
	for (const ip_t& ip : parsed_ips_reversed)
	{
		std::cout << ip << std::endl;
	}

	//2. Сразу следом список адресов, первый байт которых равен 1. Порядок сортировки не меняется. Одна строка - один адрес. Списки ничем не разделяются.
	for (const ip_t& ip : parsed_ips_reversed)
	{
		if (std::get<0>(ip) == 1)
			std::cout << ip << std::endl;
	}

	//3. Сразу продолжается список адресов, первый байт которых равен 46, а второй 70. Порядок сортировки не меняется.Одна строка - один адрес.Списки ничем не разделяются.
	for (const ip_t& ip : parsed_ips_reversed)
	{
		if (std::get<0>(ip) == 45 && std::get<1>(ip) == 70)
			std::cout << ip << std::endl;
	}

	//4. Сразу продолжается список адресов, любой байт которых равен 46. Порядок сортировки не меняется.Одна строка - один адрес.Списки ничем не разделяются.
	for (const ip_t& ip : parsed_ips_reversed)
	{
		if (std::get<0>(ip) == 45 || std::get<1>(ip) == 45 || std::get<2>(ip) == 45 || std::get<3>(ip) == 45)
			std::cout << ip << std::endl;
	}

	return 0;
}