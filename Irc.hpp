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

class Users
{
    public:
        Users(void) {};
        ~Users(void) {};

        std::vector<User> tab;
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

    public:
        Polls(void) {};
        Polls(int fd);
        ~Polls(void) {};

        void    mainPoll(void);
        void    createClientPoll(void);
};

#endif