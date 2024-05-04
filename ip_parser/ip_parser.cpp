#include "ip_parser.h"

#include <iostream>
#include <regex>
#include <boost/format.hpp>

#ifdef _DEBUG
#define DBG_LOG(code) code
#else
#define DBG_LOG(code)
#endif

bool ip_parser::parse(const std::string& line, ip_t& ip)
{
	DBG_LOG(std::cout << "Line: " << line << "..." << std::endl);

	//										___111	  .222     .333     .999______
	static const std::string ip_pattern_str = R"(\s*(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))";

	//													_____\t___text2__\t__text3__\n
	static const std::regex line_pattern(ip_pattern_str + R"(\s*\t\s*\w*\s*\t\s*\w*\s*$)");

	if (!std::regex_match(line, line_pattern))
	{
		DBG_LOG(std::cout << "Line: wrong. " << std::endl);
		return false;
	}
	else {
		DBG_LOG(std::cout << "Line: good. " << std::endl);
	}

	std::smatch matches;
	if (std::regex_search(line, matches, line_pattern))
	{
		for (size_t i = 1; i < matches.size(); ++i)
		{
			DBG_LOG(std::cout << "Line: matches[" << i << "] = " << matches[i] << std::endl);
		}

		int c1 = std::stoi(matches[1].str());
		int c2 = std::stoi(matches[2].str());
		int c3 = std::stoi(matches[3].str());
		int c4 = std::stoi(matches[4].str());

		for (int c : { c1, c2, c3, c4 })
		{
			if (c >= 0 && c <= 255)
			{

			}
			else
			{
				DBG_LOG(std::cout << L"IP item is out of [0, 255]." << std::endl);
				return false;
			}
		}

		std::get<0>(ip) = c1;
		std::get<1>(ip) = c2;
		std::get<2>(ip) = c3;
		std::get<3>(ip) = c4;

		DBG_LOG(std::cout << "Line: parsed OK. IP is: " << (boost::format("%i.%i.%i.%i") % c1 % c2 % c3 % c4).str() << std::endl);

		return true;
	}
	else {
		DBG_LOG(std::cout << "Line: matches not found. " << std::endl);
	}

	return false;
}