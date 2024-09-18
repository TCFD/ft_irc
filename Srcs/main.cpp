#include "../Includes/Server.hpp"
// #include "../Client.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		Server server(atoi(av[1]), av[2]);
		std::cout << "Server is running on port " << server.getPort() << std::endl;
		Polls poll(server.getServerSocket());
		server.setPoll(poll);
		poll.mainPoll(server);
	}
	catch (const StrerrorException& e)
	{
		std::cout << e.what() << std::endl; exit(EXIT_FAILURE);
	}
  
	return 0;
}
