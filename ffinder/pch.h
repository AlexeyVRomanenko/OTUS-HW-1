#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <filesystem>
namespace fs = std::filesystem;
#include <boost/iostreams/device/mapped_file.hpp>
namespace io = boost::iostreams;