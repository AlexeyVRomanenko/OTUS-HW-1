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

		std::vector<std::vector<std::string>> m_dirs_equal_files;
	};
}