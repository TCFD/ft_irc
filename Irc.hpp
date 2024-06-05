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
    std::string nickName;
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

    public:
        Polls(void) {};
        Polls(int fd);
        ~Polls(void) {};

        void    mainPoll(void);
        void    createClientPoll(void);
   		void	clientDisconnected(int bytes_received, int index);
		void	handleClientCommand(int client_fd, const std::string& command, int index);
		void	sendResponse(int client_fd, const std::string& response);
        void    modesHandle(int index, const std::string& command);

};

#endif