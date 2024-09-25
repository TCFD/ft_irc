#include "../../Includes/Server.hpp"

// Check if the current channel exists in the database tabChan<CHANNEL>
bool	Server::isChanExists(std::string target)
{
	int len=0;

	if (target[0] != '#')
		target = "#" + target;
	if (!_channels.empty()) {
		for (CHAN_ITC it = _channels.begin(); it != _channels.end(); len++, it++)
		{
			if (target == it->gName()) {
				_msg.currentChan = len;
				return true; }
		}
	}
	_msg.currentChan = len;
	return false;
}

// Check if the current user exists in the database tab<USER>
bool	is_user_exists(std::string target, CLIENT_VEC clients)
{
	for (CLIENT_ITC it=clients.begin(); it != clients.end(); ++it) {
		if (target == it->getNickname())
			return true;
	}
	return false;
}

// Check if the target is already in the channel or not
bool	is_user_in_chan(std::string target, Channel & chan)
{	
	// if (!chan)
	for (CLIENT_ITC it=chan.gClients().begin(); it != chan.gClients().end(); ++it) {
		if (it->getNickname() == target) {
			return true; }
	}
	return false;
}

// Check if the mode is already activated in the channel or not
bool	found_mode_in_chan(char mod, CHAR_VEC modList)
{
	for (CHAR_VEC::iterator it=modList.begin(); it != modList.end(); ++it) {
		if (*it == mod)
			return true;
	}
	return false;
}

/* 
 * Check if the third arg is +k, +l or +/-o
*/
bool	is_four_args(STR_VEC& split)
{
	if (split.size() > 2 && (split[2] == "+k" || split[2] == "-k"|| split[2] == "+l" || split[2] == "+o" || split[2] == "-o"))
		return true;
	return false;
}

/* 
 * Get the command line and split it per part
 * Exception when the third arg == -l --> forced split
*/
STR_VEC	Server::cutModeCommand(void)
{
	std::string delim = " ";
	STR_VEC split;
	for (size_t j=0; _msg.command.length() != 0; j++)
	{
		if (!_msg.command.find("-l")) {
			split.push_back(_msg.command.substr(0, 2));
			_msg.command.erase(0, (int)split[j].size());
		}
		else {
			split.push_back(_msg.command.substr(0, _msg.command.find(delim)));
			_msg.command.erase(0, (int)split[j].size() +1); }
	}
	return split;
}

int	user_in_chan_fd(std::string nick, CLIENT_VEC clients)
{
	int i = 0;
	for (CLIENT_ITC it = clients.begin(); it != clients.end(); i++, it++) {
		if (clients[i].getNickname() == nick)
			return (clients[i].getFd()); 
	}
	return (-1);
}

bool	is_user_an_operator(std::string target, Channel chan)
{
	for (CLIENT_ITC it = chan.gOperators().begin(); it != chan.gOperators().end(); ++it) {
		if (target == it->getNickname())
			return true;
	}
	return false;
}

std::string		int_to_str(int nb)
{
	std::ostringstream convert;
	convert << nb;
	return convert.str();
}

std::string		char_to_str(char nb)
{
	std::ostringstream convert;
	convert << nb;
	return convert.str();
}

std::string		time_to_str(time_t nb)
{
	std::ostringstream convert;
	convert << nb;
	return convert.str();
}