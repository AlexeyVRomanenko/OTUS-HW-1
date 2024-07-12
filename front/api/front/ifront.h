#pragma once
#include <functional>

namespace front
{
	class IFront
	{
	public:
		virtual ~IFront() = default;

		virtual void ConnectToEnter(const std::function<void(const char*)>&) = 0;
		virtual void Out(const char*) = 0;

		virtual bool Run() = 0;
		virtual void Break() = 0;
	};
}