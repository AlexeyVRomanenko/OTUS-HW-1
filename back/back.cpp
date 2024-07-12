#include "pch.h"
#include <sstream>
#include <map>
#include <chrono>
using time_t_ = std::chrono::time_point<std::chrono::system_clock>;
#include <filesystem>
#include <fstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

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
		if (!_cmd)
			return;

		const std::string cmd = _cmd;
		if (cmd.empty())
			return;

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
					m_block->child = std::make_shared<Block>(m_block);
					m_work_block = m_block->child;
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
				m_work_block = m_work_block.lock()->parent;

				if (m_work_block.lock()->parent.expired())
				{
					flush();
				}
			}
		}
		else if (boost::iequals(cmd.c_str(), "exit"))
		{
			if (!m_front.expired())
			{
				m_front.lock()->Break();
			}
		}
		else if (boost::iequals(cmd.c_str(), "eof") == 0)
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
			std::map<time_t_, std::string> cmds;
			const std::function<void(const Block* block)> print_cmds = [&cmds, &print_cmds](const Block* block)
				{
					if (!block)
						return;

					for (const auto& cmd : block->cmds)
					{
						cmds.insert(cmd);
					}

					print_cmds(block->child.get());
				};

			print_cmds(m_block.get());

			std::stringstream stream;
			stream << "bulk: ";
			for (const auto& cmd : cmds)
			{
				if (&cmd != &*cmds.begin())
					stream << ", ";

				stream << cmd.second;
			}

			m_front.lock()->Out(stream.str().c_str());
			m_front.lock()->Out("\n");

			const std::string out_file = (std::filesystem::current_path() / std::to_string(std::chrono::time_point_cast<std::chrono::milliseconds>(cmds.cbegin()->first).time_since_epoch().count())).replace_extension("log").string();

			std::ofstream fstrm(out_file, std::ios::out);
			if (fstrm)
			{
				fstrm << stream.str();
			}
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