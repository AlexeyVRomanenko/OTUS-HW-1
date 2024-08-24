#pragma once
#include <functional>
#include <tasks_thread.h>

namespace front
{
	class IFront
	{
	public:
		virtual ~IFront() = default;

		virtual void ConnectToEnter(const std::function<void(threads_pool& threads, const char*)>&) = 0;
		virtual void Out(const char*) = 0;

		virtual bool Run(threads_pool&, char* buff, size_t buff_size) = 0;
		virtual void Break() = 0;
	};
}