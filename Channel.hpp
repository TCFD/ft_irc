#pragma once

#include "Client.hpp"
#include <iostream>
#include <vector>

#define CLIENT_VEC  std::vector<Client>
#define MODES_VEC   std::vector<char>

class Channel {

    public:
        Channel(std::string name);

        ~Channel();

    private:
        std::string _name;
        std::string _topic;
        std::string _password;
        CLIENT_VEC  _clients;
        CLIENT_VEC  _operators;
        MODES_VEC   _modes;
};