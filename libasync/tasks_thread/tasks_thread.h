#pragma once

#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

namespace my
{
	class work_thread
	{
	public:
		using task_t = std::function<void()>;

		work_thread() :
			m_stopped(false),
			m_thread(&work_thread::thread_func, this)
		{
		}

		void perform_task(const task_t& task)
		{
			std::lock_guard<std::mutex> guard(m_mutex);
			m_tasks.push(task);
			m_condition.notify_all();
		}

		void stop()
		{
			if (!m_stopped)
			{
				m_stopped = true;
				m_condition.notify_all();
				m_thread.join();
			}
		}

	private:
		void thread_func()
		{
			while (!m_stopped)
			{
				std::unique_lock<std::mutex> lck{ m_mutex };
				while (!m_stopped && m_tasks.empty())
					m_condition.wait(lck);

				if (m_stopped)
					break;

				task_t t = m_tasks.front();
				m_tasks.pop();
				t();
			}

			while (!m_tasks.empty())
			{
				task_t t = m_tasks.front();
				m_tasks.pop();
				t();
			}
		}

		std::condition_variable m_condition;
		std::queue<task_t> m_tasks;
		std::mutex m_mutex;
		std::thread m_thread;
		std::atomic<bool> m_stopped;
	};
}

using work_thread_ptr = std::shared_ptr<my::work_thread>;
using threads_pool = std::array<work_thread_ptr, 3>;