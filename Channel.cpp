#include "Channel.hpp"

void    Channel::addModes(char mod)
{
    _modes.push_back(mod);
}

void    Channel::addOperator(Client& client)
{
    _operators.push_back(client);
}

void    Channel::addClient(Client& client)
{
    _clients.push_back(client);
}