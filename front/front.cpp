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

	virtual void ConnectToEnter(const std::function<void(const char*)>& fn) override
	{
		m_fn_enter = fn;
	}

	virtual void Out(const char* out) override
	{
		std::cout << out;
	}

	virtual bool Run() override
	{
		while (true)
		{
			if (m_break)
				break;

			std::string str;
			std::cin >> str;
			if (m_fn_enter)
				m_fn_enter(str.c_str());

			if (m_break)
				break;
		}		

		return true;
	}

	virtual void Break() override
	{
		m_break = true;
	}

private:
	bool m_break = false;
	std::function<void(const char*)> m_fn_enter;
};

std::shared_ptr<front::IFront> front::CreateFront()
{
	return std::make_shared<Front>();
}