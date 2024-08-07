#pragma once
#include <ffinder/fwd.h>
#include <memory>
#include <boost/noncopyable.hpp>

namespace ffinder
{
	static constexpr std::string_view DIR = "dir";
	static constexpr std::string_view DIR_EXCEPT = "dir_except";
	static constexpr std::string_view RECURSION = "recursion";
	static constexpr std::string_view MIN_FILE_SIZE = "min_file_size";
	static constexpr std::string_view FILE_MASK = "file_mask";
	static constexpr std::string_view BLOCK_SIZE = "blok_size";
	static constexpr std::string_view HASH_ALORITHM = "hash_alghoritm";

	class FilesDuplicateFinder :
		private boost::noncopyable
	{
	public:
		static std::shared_ptr<FilesDuplicateFinder> Instance(const char* cmd_line);
		FilesDuplicateFinder();

		std::string GetOutput() const;

	private:
		bool Init(const char* cmd_line);

		std::vector<std::vector<fs::path>> m_dirs_equal_files;
	};

	////////////////////////////////////////////////////////////////////////////////////

	class FilesComparer
	{
	public:
		FilesComparer(const std::vector<fs::path>& files, int block_size);

		const std::set<fs::path>& GetEqualFiles() const;

	private:
		using block_t = std::vector<char>;
		using block_hash_t = size_t;
		using blocks_t = std::vector<block_hash_t>;

		const int m_block_size = 100;

		static block_hash_t get_block_hash(const block_t&);
		block_t read_next_file_block(const fs::path&);

		std::unordered_map<fs::path, blocks_t> m_file_blocks;
		std::unordered_map<fs::path, std::fstream> m_files;

		//result
		std::set<fs::path> m_equal_files;
	};
}