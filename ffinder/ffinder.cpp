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
			(BLOCK_SIZE.data(), po::value<int>(), "Block size for files reading")
			(HASH_ALORITHM.data(), po::value<char>(), "Hash algorithm");

#ifdef _WIN32
		po::command_line_parser po_parser(po::split_winmain(cmd_line));
#else
		po::command_line_parser po_parser(po::split_unix(cmd_line));
#endif
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

	std::unordered_set<fs::path> _DIRs;
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
						_DIRs.insert(d.path());
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

	int _MIN_FILE_SIZE = 8;
	if (auto opt = opts.find(MIN_FILE_SIZE.data()); opt != opts.end())
	{
		_MIN_FILE_SIZE = opt->second.as<int>();
	}

	int _BLOCK_SIZE = 1000;
	if (auto opt = opts.find(BLOCK_SIZE.data()); opt != opts.end())
	{
		_BLOCK_SIZE = opt->second.as<int>();
	}

	std::vector<std::string> _FILE_MASKs;
	if (auto opt = opts.find(FILE_MASK.data()); opt != opts.end())
	{
		_FILE_MASKs = opt->second.as<std::vector<std::string>>();
	}

	std::vector<std::vector<fs::path>> filtered_dirs_files;
	for (const auto& dir : _DIRs)
	{
		std::vector<fs::path> files_to_compare;
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
								files_to_compare.push_back(f.path());
							}
						}
					}
					else
					{
						files_to_compare.push_back(f.path());
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

	bool compare_full_data = false;
	if (compare_full_data)
	{
		const auto FILES_COMPARER = [](const fs::path& file1, const fs::path& file2)
			{
				io::mapped_file_source f1(file1.string());
				io::mapped_file_source f2(file2.string());
				return  f1.size() == f2.size() && std::equal(f1.data(), f1.data() + f1.size(), f2.data());
			};

		for (const auto& dir : filtered_dirs_files)
		{
			std::unordered_set<fs::path> checker;
			std::vector<fs::path> equal_files;
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
	}
	else
	{
		for (const auto& dir : filtered_dirs_files)
		{
			FilesComparer comparer(dir, _BLOCK_SIZE);

			std::set<fs::path> equal_files = comparer.GetEqualFiles();
			if (!equal_files.empty())
			{
				m_dirs_equal_files.push_back({ equal_files.begin(), equal_files.end() });
			}
		}
	}

	return true;
}

std::string FilesDuplicateFinder::GetOutput() const
{
	fs::path out;
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

	return out.string(); //rvo
}

///////////////////////////////////////////////////////////////////////////////////////////////

FilesComparer::FilesComparer(const std::vector<fs::path>& files, int block_size) :
	m_block_size(block_size)
{
	for (const auto& file1 : files)
	{
		if (m_equal_files.contains(file1))
			continue;

		for (const auto& file2 : files)
		{
			if (m_equal_files.contains(file2))
				continue;

			if (file1 == file2)
				continue;

			blocks_t& blocks1 = m_file_blocks[file1];
			blocks_t& blocks2 = m_file_blocks[file2];

			unsigned int block_index = 0;
			while (true)
			{
				//file1
				block_hash_t block_hash1 = 0;
				if (blocks1.size() <= block_index) {

					const block_t next_block = read_next_file_block(file1);
					if (next_block.empty()) {
						block_hash1 = 0;
					}
					else {
						block_hash1 = get_block_hash(next_block);
						blocks1.push_back(block_hash1);
					}
				}
				else {
					block_hash1 = blocks1[block_index];
				}

				//file2
				block_hash_t block_hash2 = 0;
				if (blocks2.size() <= block_index) {
					const block_t next_block = read_next_file_block(file2);
					if (next_block.empty()) {
						block_hash2 = 0;
					}
					else
					{
						block_hash2 = get_block_hash(next_block);
						blocks2.push_back(block_hash2);
					}
				}
				else {
					block_hash2 = blocks2[block_index];
				}

				if (block_hash1 != block_hash2)
				{
					//!not equal
					break;
				}

				if (block_hash1 == 0 && block_hash2 == 0)
				{
					//!equal
					m_equal_files.insert({ file1, file2 });
					break;
				}

				block_index++;
			}
		}
	}
}

FilesComparer::block_t FilesComparer::read_next_file_block(const fs::path& file)
{
	std::fstream& fstr = m_files[file];
	if (!fstr.is_open())
	{
		fstr.open(file);
	}
	else
	{
		if (fstr.eof())
		{
			return {};
		}
	}

	std::vector<char> block(m_block_size, 0);
	fstr.read(block.data(), block.size());
	return block;
}

FilesComparer::block_hash_t FilesComparer::get_block_hash(const block_t& block)
{
	size_t seed = 0;
	boost::hash_combine(seed, block);
	return seed;
}

const std::set<fs::path>& FilesComparer::GetEqualFiles() const
{
	return m_equal_files;
}