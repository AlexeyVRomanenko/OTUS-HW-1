#include "pch.h"
#include <sstream>
#include <map>
#include <chrono>
using time_t_ = std::chrono::time_point<std::chrono::system_clock>;

#include <back/back.h>
#include <front/ifront.h>

class Back :
	public back::IBack
{
public:
	Back(const std::weak_ptr<front::IFront>& front, uint32_t N) :
		m_front(front),
		m_N(N)
	{
		if (!m_front.expired())
			m_front.lock()->ConnectToEnter(std::bind(&Back::OnEnter, this, std::placeholders::_1));
	}

private:
	void OnEnter(const char* _cmd)
	{
		if (!_cmd || strlen(_cmd) == 0)
			return;

		const std::string cmd = _cmd;

		if (cmd == "{")
		{
			if (!m_work_block.expired() && m_work_block.lock()->parent.expired())
			{
				flush();
			}

			{
				if (!m_block)
				{
					m_block = std::make_shared<Block>(std::weak_ptr<Block>());
					//m_block->child = std::make_shared<Block>(m_block);
					m_work_block = m_block/*->child*/;
				}
				else
				{
					m_work_block.lock()->child = std::make_shared<Block>(m_work_block.lock());
					m_work_block = m_work_block.lock()->child;
				}
			}
		}
		else if (cmd == "}")
		{
			if (!m_work_block.expired())
			{
				if (m_work_block.lock()->parent.expired())
				{
					flush();
				}
				else
				{
					m_work_block = m_work_block.lock()->parent;
				}
			}
		}
		else if (stricmp(cmd.c_str(), "exit") == 0)
		{
			if (!m_front.expired())
			{
				m_front.lock()->Break();
			}
		}
		else if (stricmp(cmd.c_str(), "eof") == 0)
		{
			flush();
		}
		else
		{
			if (m_work_block.expired())
			{
				m_block = std::make_shared<Block>(std::weak_ptr<Block>());
				m_work_block = m_block;
			}

			m_work_block.lock()->cmds.insert({ std::chrono::system_clock::now(), cmd });

			if (m_work_block.lock()->parent.expired() && m_work_block.lock()->cmds.size() == m_N)
			{
				flush();
			}
		}
	}

	void flush()
	{
		if (!m_front.expired())
		{
			std::stringstream stream;
			stream << "bulk: ";

			const std::function<void(const Block* block)> print_cmds = [&stream, &print_cmds](const Block* block)
				{
					if (!block)
						return;

					for (const auto& cmd : block->cmds)
					{
						stream << cmd.second << "\t";
					}

					print_cmds(block->child.get());
				};

			print_cmds(m_block.get());

			m_front.lock()->Out(stream.str().c_str());
			m_front.lock()->Out("\n");
		}

		m_block.reset();
	}

	struct Block
	{
		Block(const std::weak_ptr<Block>& p) :
			parent(p)
		{
		}

		std::map<time_t_, std::string> cmds;
		std::shared_ptr<Block> child;
		std::weak_ptr<Block> parent;
	};

	std::shared_ptr<Block> m_block;
	std::weak_ptr<Block> m_work_block;

private:
	const uint32_t m_N;
	std::weak_ptr<front::IFront> m_front;
};

std::shared_ptr<back::IBack> back::CreateBack(const std::weak_ptr<front::IFront>& front, uint32_t N)
{
	return std::make_shared<Back>(front, N);
}