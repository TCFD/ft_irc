#include "Server.hpp"
#include "Client.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    try {
        Server server(atoi(av[1]));
        std::cout << "Server is running on port " << server.getPort() << std::endl;
        Polls poll(server.getServerSocket());
		poll.mainPoll();
    }
    catch (const StrerrorException& e)
    { std::cout << e.what() << std::endl; exit(EXIT_FAILURE); }
  
    return 0;
}
