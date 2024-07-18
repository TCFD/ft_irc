#include "Channel.hpp"

Channel::Channel(std::string name): _name(name)
{
    _lenClients = 0;
	_limit = 0;
}

void    Channel::addMode(char mod)
{
    _modes.push_back(mod);
	// std::cout << GREEN << _modes.back()  << NC << std::endl; 
}

void    Channel::addOperator(Client& client)
{
    _operators.push_back(client);
}

bool	Channel::isUserOnMe(std::string nick)
{
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

void	Channel::dltMode(char mod)
{
	for (MODES_VEC::iterator it = _modes.begin(); it != _modes.end(); ++it)
	{
		if (*it == mod) {
			_modes.erase(it); return ;}
	}
}

void	Channel::dltOperator(Client& client)
{
	for (CLIENT_IT it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (it->getNickname() == client.getNickname()) {
			_operators.erase(it); return ; }
	}
}
