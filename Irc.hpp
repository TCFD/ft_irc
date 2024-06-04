#ifndef IRC_HPP
# define IRC_HPP

# include <cstdlib>
# include <cstdio>
# include <fcntl.h>
# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <map>
# include <vector>
# include <exception>
# include <errno.h>

# include "Server.hpp"
# include "Client.hpp"

struct User {
    int indexInPollFd;
    std::string userName;
};

class Polls
{
    private:
        std::vector<struct pollfd> pollFds;
        struct pollfd clientPollFds;
        struct pollfd serverPollFds;
        int serverFd;
        int pollCount;

        std::map<int, std::string> clientsBuffer;
		std::vector<User> tab;

		void	clientDisconnected(int bytes_received, int index);
		void	handle_client_command(int client_fd, const std::string& command, int index);
		void	send_response(int client_fd, const std::string& response);
    public:
        Polls(void) {};
        Polls(int fd);
        ~Polls(void) {};

        void    mainPoll(void);
        void    createClientPoll(void);
};

#endif