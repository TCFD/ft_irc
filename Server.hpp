#pragma once

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
# include <algorithm>

# include "Irc.hpp"

class Server
{
    protected:
        int                 _port;
        int                 _serverSocket;
        struct sockaddr_in  _serverAddr;
        int                 _limitUsers;
        CLIENT_VEC          _clients;
        CHAN_VEC            _channels;
    
    public:
        Server(int port);
        ~Server(void) {};

        int             getPort(void) {return(_port); }
        int             getServerSocket(void) { return(_serverSocket); }
        void            socketDataSet(void);

        void            createClient(Polls &poll);
   		void	        clientDisconnected(int bytes_received);


};

class StrerrorException : public std::exception
{
    private:
        char bufferMessage[256];

    public :
        explicit StrerrorException(const char* message){
            snprintf(bufferMessage, sizeof(bufferMessage), "%s: %s", message, strerror(errno)); }

        virtual const char* what() const throw(){
            return bufferMessage; };
};

class Polls
{
    private:
        std::vector<struct pollfd>		_pollFds;
        struct pollfd					_clientPollFds;
        struct pollfd					_serverPollFds;
        int								_pollCount;
        Msg                             _msg;

    public:

        Polls(int fd) {};
        ~Polls() {};

        void            mainPoll(Server& server);
        void            addClientPoll(int clientFd);

};