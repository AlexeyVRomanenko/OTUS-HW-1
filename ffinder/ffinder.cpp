#include "pch.h"
#include <ffinder/ffinder.h>

using namespace ffinder;

std::shared_ptr<FilesDuplicateFinder> FilesDuplicateFinder::Instance(const char* cmd_line)
{
	if (!(cmd_line && std::strlen(cmd_line)))
		return {};

	auto finder = std::make_shared<FilesDuplicateFinder>();
	if (!finder->Init(cmd_line))
		return {};

	return finder;
}

FilesDuplicateFinder::FilesDuplicateFinder()
{
}

bool FilesDuplicateFinder::Init(const char* cmd_line)
{
	po::variables_map opts;

	{
		po::options_description po_desc("Options");
		po_desc.add_options()
			(DIR.data(), po::value<std::vector<std::string>>()->multitoken(), "Directory for scanning")
			(DIR_EXCEPT.data(), po::value<std::vector<std::string>>()->multitoken(), "Directory except")
			(RECURSION.data(), po::value<int>(), "level of scanning")
			(MIN_FILE_SIZE.data(), po::value<int>(), "minimal file size")
			(FILE_MASK.data(), po::value<std::vector<std::string>>()->multitoken(), "Mask of file allowed for scanning")
			(BLOCK_SIZE.data(), po::value<char>(), "Block size for files reading")
			(HASH_ALORITHM.data(), po::value<char>(), "Hash algorithm");

		po::command_line_parser po_parser(po::split_winmain(cmd_line));
		po_parser
			.options(po_desc)
			.allow_unregistered()
			.style(po::command_line_style::default_style |
				po::command_line_style::allow_slash_for_short |
				po::command_line_style::short_case_insensitive |
				po::command_line_style::long_case_insensitive);

		po::store(po_parser.run(), opts);
		po::notify(opts);
	}

	/////////////////////////////////////////////

	int _RECURSION = 0;
	if (auto opt = opts.find(RECURSION.data()); opt != opts.end())
	{
		_RECURSION = opt->second.as<int>();
	}

	std::unordered_set<std::string> _DIRs;
	if (auto opt = opts.find(DIR.data()); opt != opts.end())
	{
		std::vector<std::string> dirs = opt->second.as<std::vector<std::string>>();
		_DIRs.insert(dirs.cbegin(), dirs.cend());
		if (_RECURSION == 1)
		{
			for (const auto& dir : dirs)
			{
				for (const fs::directory_entry& d : fs::recursive_directory_iterator(dir))
				{
					if (d.is_directory())
						_DIRs.insert(d.path().string());
				}
			}
		}
	}

	if (auto opt = opts.find(DIR_EXCEPT.data()); opt != opts.end())
	{
		for (const auto& dir_ex : opt->second.as<std::vector<std::string>>())
		{
			_DIRs.erase(dir_ex);
		}
	}

	int _MIN_FILE_SIZE = 0;
	if (auto opt = opts.find(MIN_FILE_SIZE.data()); opt != opts.end())
	{
		_MIN_FILE_SIZE = opt->second.as<int>();
	}

	std::vector<std::string> _FILE_MASKs;
	if (auto opt = opts.find(FILE_MASK.data()); opt != opts.end())
	{
		_FILE_MASKs = opt->second.as<std::vector<std::string>>();
	}

	std::vector<std::vector<std::string>> filtered_dirs_files;

	for (const auto& dir : _DIRs)
	{
		std::vector<std::string> files_to_compare;
		for (const fs::directory_entry& f : fs::directory_iterator(dir))
		{
			if (f.is_regular_file())
			{
				if (fs::file_size(f) >= _MIN_FILE_SIZE)
				{
					if (!_FILE_MASKs.empty())
					{
						for (const auto& mask : _FILE_MASKs)
						{
							if (f.path().stem().string().find(mask) != std::string::npos)
							{
								files_to_compare.push_back(f.path().string());
							}
						}
					}
					else
					{
						files_to_compare.push_back(f.path().string());
					}
				}
			}
		}

		if (!files_to_compare.empty())
		{
			filtered_dirs_files.emplace_back(std::move(files_to_compare));
		}
	}

	char _HASH_ALORITHM = 0;
	if (auto opt = opts.find(HASH_ALORITHM.data()); opt != opts.end())
	{
		_HASH_ALORITHM = opt->second.as<char>();
	}

	const auto FILES_COMPARER = [](const std::string& file1, const std::string& file2)
		{
			io::mapped_file_source f1(file1);
			io::mapped_file_source f2(file2);
			return  f1.size() == f2.size() && std::equal(f1.data(), f1.data() + f1.size(), f2.data());
		};

	for (const auto& dir : filtered_dirs_files)
	{
		std::unordered_set<std::string> checker;
		std::vector<std::string> equal_files;
		for (const auto& file1 : dir)
		{
			for (const auto& file2 : dir)
			{
				if (file1 == file2)
					continue;

				if (FILES_COMPARER(file1, file2))
				{
					if (checker.contains(file1))
						continue;

					equal_files.push_back(file1);
					equal_files.push_back(file2);
					checker.insert({ file1, file2 });
				}
			}
		}

		if (!equal_files.empty())
		{
			m_dirs_equal_files.emplace_back(std::move(equal_files));
		}
	}

	return true;
}

std::string FilesDuplicateFinder::GetOutput() const
{
	std::string out;
	for (const auto& dir : m_dirs_equal_files)
	{
		for (const auto& file : dir)
		{
			if (!out.empty())
				out += "\n";

			out += file;
		}

		out += "\n";
	}

	return out; //rvo
}