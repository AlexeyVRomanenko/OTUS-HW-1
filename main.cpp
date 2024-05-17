#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <execution>
#include <atomic>

#include <boost/filesystem.hpp>

#include <tbb/task_scheduler_observer.h>
#include <tbb/parallel_for.h>
#include <tbb/task.h>
#include <tbb/partitioner.h>

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
	{
		ips_parsed.resize(lines.size());

		std::atomic_bool success(true);
		tbb::parallel_for(tbb::blocked_range(size_t(0), lines.size()),
			[&lines, &ips_parsed, &success](const auto& range)
			{
				for (size_t i = range.begin(); i != range.end(); ++i)
				{
					if (!ip_parser::parse(lines[i], ips_parsed[i]))
					{
						success = false;
					}
				}
			}
		);
		if (!success)
			return 1;
	}

	std::vector<ip_t> parsed_ips_reversed, ips_sorted;
	{
		ips_sorted = ips_parsed;
		std::sort(ips_sorted.begin(), ips_sorted.end());

		parsed_ips_reversed.resize((ips_sorted.size()));
		std::reverse_copy(ips_sorted.begin(), ips_sorted.end(), parsed_ips_reversed.begin());
	}

	{
		const std::string out_file = fs::current_path().string() + R"(/parsed_ips.txt)";

		std::ofstream fstrm(out_file, std::ios::out);
		if (!fstrm)
			return 1;

		//1. Полный список адресов после сортировки. Одна строка - один адрес
		for (const ip_t& ip : parsed_ips_reversed)
		{
			std::cout << ip << std::endl;
			fstrm << ip << std::endl;
		}

		//2. Сразу следом список адресов, первый байт которых равен 1. Порядок сортировки не меняется. Одна строка - один адрес. Списки ничем не разделяются.
		for (const ip_t& ip : parsed_ips_reversed)
		{
			if (std::get<0>(ip) == 1) {
				std::cout << ip << std::endl;
				fstrm << ip << std::endl;
			}
		}

		//3. Сразу продолжается список адресов, первый байт которых равен 46, а второй 70. Порядок сортировки не меняется.Одна строка - один адрес.Списки ничем не разделяются.
		for (const ip_t& ip : parsed_ips_reversed)
		{
			if (std::get<0>(ip) == 46 && std::get<1>(ip) == 70) {
				std::cout << ip << std::endl;
				fstrm << ip << std::endl;
			}
		}

		//4. Сразу продолжается список адресов, любой байт которых равен 46. Порядок сортировки не меняется.Одна строка - один адрес.Списки ничем не разделяются.
		for (const ip_t& ip : parsed_ips_reversed)
		{
			if (std::get<0>(ip) == 46 || std::get<1>(ip) == 46 || std::get<2>(ip) == 46 || std::get<3>(ip) == 46) {
				std::cout << ip << std::endl;
				fstrm << ip << std::endl;
			}
		}

		fstrm.flush();

		// .\md5sum.exe .\parsed_ips.txt
		// 9640392a0eae8f98ad807c5538cd3148 *parsed_ips.txt
	}

	return 0;
}