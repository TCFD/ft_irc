#pragma once

# include "Irc.hpp"

class Client
{
    private:
        std::string username;
        std::string nickname;
        std::string realname;
        std::string connectedTo;

    public:
        Client(void) {};
        ~Client(void) {};

        std::string getUsername(void) {return username;};
        std::string getNickname(void) {return nickname;};
        std::string getRealname(void) {return realname;};
        std::string getConnectedTo(void) {return connectedTo;};

        int parseNickname(std::string);
};

// class ClientPoll
// {
//     protected:
//         struct pollfd clientPollFds;

//     public:
//         ClientPoll(void) {};
//         ClientPoll(int clientFd);
//         ~ClientPoll(void) {};
// };
    