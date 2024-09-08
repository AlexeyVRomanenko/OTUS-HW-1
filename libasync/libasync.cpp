#include <libasync.h>
#include <array>
#include <front/front.h>
#include <front/ifront.h>
#include <back/back.h>
#include <tasks_thread/tasks_thread.h>

static libasync::context_t context = 1000;
static std::map<libasync::context_t, std::pair<std::shared_ptr<front::IFront>, std::shared_ptr<back::IBack>>> _instances;

static threads_pool _threads;

libasync::context_t libasync::connect(uint32_t N)
{
	if (_instances.empty())
	{
		//run threads
		for (auto& wt : _threads) {
			wt = std::make_shared<my::work_thread>();
		}
	}

	context++;

	std::shared_ptr<front::IFront> front = front::CreateFront();
	std::shared_ptr<back::IBack> back = back::CreateBack(front, N);
	_instances[context] = { front , back };
	return context;
}

bool libasync::receive(context_t c, const char* buff, size_t buff_size)
{
	return _instances[c].first->Run(_threads, buff, buff_size);
}

bool libasync::disconnect(context_t c)
{
	_instances[c].first->Break();
	_instances.erase(c);

	if (_instances.empty())
	{
		//stop threads
		for (auto& wt : _threads) {
			wt->stop();
			wt.reset();
		}
	}

	return true;
}