#include "Channel.hpp"

Channel::Channel(std::string name): _name(name)
{
    _lenClients = 0;
	_limit = 0;
}

void    Channel::addModes(char mod)
{
    _modes.push_back(mod);
}

void    Channel::addOperator(Client& client)
{
    _operators.push_back(client);
}

bool	Channel::isUserOnMe(std::string nick) {
	CLIENT_IT it = _clients.begin();

	for (; it != _clients.end(); it++) {
		if (it->getNickname() == nick) {
			return true;
		}
	}
	return false;
}

void    Channel::addClient(Client& client)
{
    _clients.push_back(client);
}

void	Channel::addLenClient(void)
{
	_lenClients ++;
}

void	Channel::dltLenClient(void)
{
	_lenClients --;
}