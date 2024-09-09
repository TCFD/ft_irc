#include "../Channel.hpp"

Channel::Channel(std::string name): _name(name)
{
	_lenClients = 0;
	_limit = 0;
}

void	Channel::addMode(char mod)
{
	_modes.push_back(mod);
	std::cout << GREEN << _modes.back()  << NC << std::endl; 
}

void	Channel::addOperator(Client& client)
{
	_operators.push_back(client);
}

void	Channel::addOperatorByName(std::string name, CLIENT_VEC cli)
{
	int len=0;
	for (CLIENT_IT it = _clients.begin(); it != _clients.end(); it++, len++)
	{
		if (it->getNickname() == name)
			break ;
	}
	_operators.push_back(cli[len]);
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

void	Channel::addClient(Client& client)
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

void	Channel::dltOperator(std::string name)
{
	for (CLIENT_IT it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (it->getNickname() == name) {
			_operators.erase(it); return ; }
	}
}

// std::string	Channel::getTime(void)
// {
// 	std::time_t currentTime = std::time(0);
// 	std::tm*	localTime = std::localtime(&currentTime);
// 	char buffer[100];

// 	std::strftime(buffer, sizeof(buffer), "%a %b  %d %H:%M:%S %Y", localTime);
	
// 	return std::string(buffer); 
// }

std::string		Channel::gModesActives(void) {
	std::string mods = "+";
	int limit=0;
	int key=0;

	for (MODES_VEC::iterator it = _modes.begin(); it != _modes.end(); ++it) {
	
		if (*it == 'k')
			key = 1;
		else if (*it == 'l')
			limit = 1;
		else
			mods += *it; 
	}
	
	if (limit == 1 && key == 1)
		mods += "lk " + intToStr(_limit) + " " + _password;
	else if (key == 1)
		mods += "k " + _password;
	else if (limit == 1)
		mods += "l " + intToStr(_limit);
	return mods;
}