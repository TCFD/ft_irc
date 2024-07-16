#pragma once

#include "Client.hpp"
#include "Irc.hpp"
#include <iostream>
#include <vector>

class Client;

# define CLIENT_VEC         std::vector<Client>
# define MODES_VEC          std::vector<char>
# define CLIENT_IT          CLIENT_VEC::iterator
# define CHAN_IT            CHAN_VEC::iterator

class Channel {

    private:
        std::string _name;
        std::string _topic;
        std::string _password;
        int         _limit;
        CLIENT_VEC  _clients;
        CLIENT_VEC  _operators;
        MODES_VEC   _modes;

    public:
        Channel(std::string name): _name(name) {};
        ~Channel() {};

        std::string     gName() { return _name; };
        std::string     gTopic() { return _topic; };
        std::string     gPassword() { return _password; };
        int             gLimit() {return _limit; };
        MODES_VEC       gModes() { return _modes; };
        CLIENT_VEC      gClients() { return _clients; };
        Client&         gClientIdx(int idx) { return _clients[idx]; };

        void            sName(std::string name) {_name = name; };
        void            sTopic(std::string topic) {_topic = topic; };
        void            sPwd(std::string pwd) {_password = pwd; };
        int             sLimit(void) { return _limit; };

        void            addModes(char mod);
        void            addOperator(Client& client);
        void            addClient(Client& client);
};