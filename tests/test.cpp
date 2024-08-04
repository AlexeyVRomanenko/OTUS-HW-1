#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>
#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <boost/format.hpp>

#include <ffinder/ffinder.h>

BOOST_AUTO_TEST_CASE(Test_1)
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_level::log_messages);

	std::string cmd_line;

	{
		const fs::path test_dirs = fs::current_path() / "test_dirs";

		//1 ffinder::DIR
		cmd_line += (boost::format(R"(--%s="%s")") % ffinder::DIR.data() % test_dirs.string()).str();
		for (const fs::directory_entry& dir : fs::directory_iterator(test_dirs))
		{
			if (dir.is_directory())
			{
				if (!cmd_line.empty())
					cmd_line += " ";

				cmd_line += (boost::format(R"(--%s="%s")") % ffinder::DIR.data() % dir.path().string()).str();
			}			
		}

		//2 ffinder::DIR_EXCEPT
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s="%s")") % ffinder::DIR_EXCEPT.data() % fs::directory_iterator(test_dirs)->path().string()).str();
		}

		//3 ffinder::RECURSION
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s=%i)") % ffinder::RECURSION.data() % 1).str();
		}

		//4 ffinder::MIN_FILE_SIZE
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s=%i)") % ffinder::MIN_FILE_SIZE.data() % 19539).str();
		}

		//5 ffinder::FILE_MASK
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s="%s")") % ffinder::FILE_MASK.data() % "wincmd").str();
		}

		//6 ffinder::BLOCK_SIZE
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s=%s)") % ffinder::BLOCK_SIZE.data() % "S").str();
		}

		//6 ffinder::HASH_ALORITHM
		{
			if (!cmd_line.empty())
				cmd_line += " ";

			cmd_line += (boost::format(R"(--%s=%s)") % ffinder::HASH_ALORITHM.data() % "H").str();
		}
	}

	auto finder = ffinder::FilesDuplicateFinder::Instance(cmd_line.c_str());

	const std::string out = finder->GetOutput();
	BOOST_TEST_MESSAGE(out.c_str());
}