#include "Channel.hpp"

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