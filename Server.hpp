#pragma once

# include "Irc.hpp"

// class ServerPoll
// {
//     protected:
//         struct pollfd   serverPollFds;

//     public:
//         ServerPoll(void) {};
//         ServerPoll(int serverFd);
//         ~ServerPoll(void) {};
// };

class Server
{
    protected:
        int port;
        int serverSocket;
        struct sockaddr_in serverAddr;
        int limitUsers;
    
    public:
        Server(void) {};
        Server(int _port);
        ~Server(void) {};

        int getPort(void) {return(port); }
        int getServerSocket(void) { return(serverSocket); }
        void    socketDataSet(void);
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