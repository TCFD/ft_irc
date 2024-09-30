#include "../Includes/Server.hpp"

bool	Polls::_quit = false;

bool	is_digit_in_str(std::string str)
{
	for (size_t i=0; i < str.size(); i++)
	{
		if (!std::isdigit(str[i])) {
			return false; }
	}
	return true;
}

int main(int ac, char **av)
{
	if (ac != 3 || (ac == 3 && !is_digit_in_str(av[1])))
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
