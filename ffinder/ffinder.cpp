#include "pch.h"
#include <ffinder/ffinder.h>

using namespace ffinder;

std::shared_ptr<FilesDuplicateFinder> FilesDuplicateFinder::Instance(const char* cmd_line)
{
	if (!(cmd_line && std::strlen(cmd_line)))
		return {};

	auto finder = std::make_shared<FilesDuplicateFinder>(cmd_line);
	if (!finder)
		return {};

	if (!finder->ParceCmdLine())
		return {};

	return finder;
}

FilesDuplicateFinder::FilesDuplicateFinder(const char* cmd_line) :
	m_cmd_line(cmd_line)
{
}

bool FilesDuplicateFinder::ParceCmdLine()
{

	return true;
}