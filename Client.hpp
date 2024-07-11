#pragma once

# include "Server.hpp"

class Client
{
    private:
        std::string _username;
        std::string _nickname;
        std::string _realname;
        std::string _oldname;
        std::string _hostname;
        bool        _registered;
        int         _fd;

    public:
        Client(int clientFd);
        ~Client(void) {};

        std::string getUsername(void) {return _username; };
        std::string getNickname(void) {return _nickname; };
        std::string getRealname(void) {return _realname; };
        std::string getOldname(void) {return _oldname; };
        bool        getRegistered(void) {return _registered; };

        int parseNickname(std::string);
};
    