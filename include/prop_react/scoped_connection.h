#pragma once

#include <boost/signals2.hpp>

template<typename T>
class LocallyInstanced
{
public:
	static std::shared_ptr<T> Create()
	{
		return std::shared_ptr<T>(new T, &LocallyInstanced::Release);
	}

private:
	static void Release(T* p)
	{
		if (p)
			delete p;
	}
};

class scoped_connection
{
	typedef boost::signals2::scoped_connection SC;
	typedef std::shared_ptr<SC> SCPtr;

public:
	static scoped_connection create(const boost::signals2::connection& c)
	{
		scoped_connection nsc;
		nsc.m_disconnector = boost::bind(&SC::disconnect, boost::placeholders::_1);
		nsc.m_sc = LocallyInstanced<SC>::Create();
		*nsc.m_sc = c;
		return nsc;
	}

	scoped_connection()
	{
	}

	scoped_connection(const scoped_connection& a)
	{
		m_disconnector = a.m_disconnector;
		m_sc = a.m_sc;
	}

	scoped_connection& operator= (const scoped_connection& a)
	{
		m_disconnector = a.m_disconnector;
		m_sc = a.m_sc;
		return *this;
	}

	scoped_connection(scoped_connection&&) = default;
	scoped_connection& operator=(scoped_connection&&) = default;

	void disconnect()
	{
		if (m_sc)
		{
			if (m_disconnector)
				m_disconnector(m_sc.get());

			m_sc.reset();
		}
	}

	bool connected() const
	{
		return m_sc ? m_sc->connected() : false;
	}

private:
	SCPtr m_sc;
	std::function<void(SC*)> m_disconnector;
};