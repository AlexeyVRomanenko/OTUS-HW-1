#pragma once
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>

using ip_t = std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>;

static std::ostream& operator<<(std::ostream& os, const ip_t& ip)
{
	os << std::get<0>(ip)
		<< "." << std::get<1>(ip)
		<< "." << std::get<2>(ip)
		<< "." << std::get<3>(ip);
	return os;
}

namespace ip_parser
{
	bool parse(const std::string& str, ip_t& ip);
}