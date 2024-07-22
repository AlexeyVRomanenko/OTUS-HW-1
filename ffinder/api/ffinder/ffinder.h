#pragma once
#include <ffinder/fwd.h>
#include <memory>
#include <boost/noncopyable.hpp>

namespace ffinder
{
	class FilesDuplicateFinder :
		private boost::noncopyable
	{
	public:
		static std::shared_ptr<FilesDuplicateFinder> Instance(const char* cmd_line);

		bool ParceCmdLine();

	private:
		FilesDuplicateFinder(const char* cmd_line);

		const std::string m_cmd_line;
	};
}