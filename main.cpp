#include <iostream>
#include <string>

#include <libasync.h>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/streambuf.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace
{
	asio::awaitable<void> work_with_client(tcp::socket socket, uint8_t bulk_n)
	{
		const libasync::context_t ctx = libasync::connect(bulk_n);

		asio::streambuf buffer;
		for (;;)
		{
			buffer.consume(buffer.size());

			try {
				co_await asio::async_read_until(socket, buffer, "\n",
					asio::use_awaitable);
			}
			catch (const boost::system::system_error& ex) {
				if (ex.code() != asio::error::eof) {
					throw;
				}
				break;
			}

			std::string cmd_str{ asio::buffer_cast<const char*>(buffer.data()), buffer.size() };
			if (!cmd_str.empty())
			{
				cmd_str.erase(cmd_str.size() - 1); //"\n" 
				if (!libasync::receive(ctx, cmd_str.data(), cmd_str.size() - 1))
					break;
			}
		}

		libasync::disconnect(ctx);
	}

	asio::awaitable<void> wait_for_clients(uint16_t port, uint16_t bulk_n)
	{
		const auto executor = co_await asio::this_coro::executor;
		tcp::acceptor acceptor{ executor, {tcp::v4(), port} };
		for (;;)
		{
			auto socket = co_await acceptor.async_accept(asio::use_awaitable);
			asio::co_spawn(executor, work_with_client(std::move(socket), bulk_n), asio::detached);
		}
	}

	void run_server(uint16_t port, uint16_t bulk_n)
	{
		asio::io_context io_context;

		asio::signal_set signals{ io_context, SIGINT, SIGTERM };
		signals.async_wait([&](auto, auto) { io_context.stop(); });

		asio::co_spawn(io_context, wait_for_clients(port, bulk_n), asio::detached);
		io_context.run();
	}
}

int main(int arc, char** arv)
{
	if (arc != 3)
	{
		std::cerr << "Wrong arguments." << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		uint16_t port = std::stoul(arv[1]);
		uint16_t bulk_n = std::stoul(arv[2]);

		std::cout << "server: " << port << " " << bulk_n << std::endl << std::endl;

		run_server(port, bulk_n);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex) {
		std::cerr << "Fatal error \"" << ex.what() << "\"." << std::endl;
	}
	catch (...) {
		std::cerr << "Fatal UNKNOWN error." << std::endl;
	}

	return EXIT_FAILURE;
}