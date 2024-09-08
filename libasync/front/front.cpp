#include "pch.h"
#include <front/front.h>
#include <front/ifront.h>

class Front :
	public front::IFront
{
public:
	Front()
	{
	}

	virtual void ConnectToEnter(const std::function<void(threads_pool&, const char*, bool&)>& fn) override
	{
		m_fn_enter = fn;
	}

	virtual void Out(const char* out) override
	{
		std::cout << out;

		{
			std::lock_guard lock_buff(m_buff_mutex);
			if (m_buff)
			{
				if (size_t size = std::strlen(out); size > 0 && m_buff_used_size + size <= m_buff_size)
				{
					std::memcpy(m_buff + m_buff_used_size, out, size);
					m_buff_used_size += size;
				}
			}
		}
	}

	virtual bool Run(threads_pool& threads, const char* buff, size_t buff_size) override
	{
		std::string str;
		if (buff) {
			str = buff;
		}
		else
			std::cin >> str;

		if (m_fn_enter)
		{
			bool _break = false;
			m_fn_enter(std::ref(threads), str.c_str(), _break);
			if (_break)
				return false;
		}

		return true;
	}

	virtual void Break() override
	{
		m_break = true;
	}

private:
	bool m_break = false;
	std::function<void(threads_pool&, const char*, bool&)> m_fn_enter;

	std::mutex m_buff_mutex;
	char* m_buff = 0;
	size_t m_buff_size = 0;
	size_t m_buff_used_size = 0;
};

std::shared_ptr<front::IFront> front::CreateFront()
{
	return std::make_shared<Front>();
}