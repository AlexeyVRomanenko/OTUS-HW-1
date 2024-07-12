#pragma once
#include <memory>

namespace front
{
	std::shared_ptr<class IFront> CreateFront();
}