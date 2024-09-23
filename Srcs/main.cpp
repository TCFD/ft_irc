#include "../Includes/Server.hpp"
// #include "../Client.hpp"

bool	Polls::_quit = false;

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		signal(SIGINT, Polls::signalHandler);
		Server server(atoi(av[1]), av[2]);
		std::cout << "Server is running on port " << server.getPort() << std::endl;
		Polls poll(server.getServerSocket());
		server.setPoll(poll);
		poll.mainPoll(server);
	}
	catch (const StrerrorException& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
