#include <iostream>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace
{
	asio::awaitable<void> send_to_server(tcp::socket socket)
	{
		for (;;)
		{
			std::string client_input;
			std::getline(std::cin, client_input);
			client_input += "\n";

			co_await asio::async_write(socket, asio::buffer(client_input), asio::use_awaitable);
		}
	}

	void run_client(uint16_t port)
	{
		asio::io_context io_context;

		asio::signal_set signals{ io_context, SIGINT, SIGTERM };
		signals.async_wait([&](auto, auto) { io_context.stop(); });

		tcp::socket socket{ io_context };
		socket.connect(tcp::endpoint(asio::ip::make_address("127.0.0.1"), port));

		asio::co_spawn(io_context, send_to_server(std::move(socket)), asio::detached);
		io_context.run();
	}
}

int main(int arc, char** arv)
{
	if (arc != 2)
		return EXIT_FAILURE;

	try {
		uint16_t port = std::stoul(arv[1]);
		std::cout << "client: " << port << std::endl << std::endl;

		run_client(port);

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