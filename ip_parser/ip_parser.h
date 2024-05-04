#pragma once
#include <string>
#include <tuple>

namespace ip_parser
{
    using ip_t = std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>;
    bool parse(const std::string& str, ip_t& ip);
}