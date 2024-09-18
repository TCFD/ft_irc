#include "../Client.hpp"

Client::Client(int clientFd)
{
	_fd			= clientFd;
	_registered	= false;
	_username	= "";
	_nickname	= "";
	_realname	= "";
	_oldname	= "";
	_id			= "";
	_hostname	= "server";
	_passwd 		= false;
}

int Client::parseNickname(std::string name)
{
	char test[5] = {'#', '&', '~', '%', '+'};
	std::vector<char> tabStart;
	for (int i = 0; i < 5; i ++) {tabStart.push_back(test[i]);}
	for (std::vector<char>::iterator it=tabStart.begin(); it != tabStart.end(); ++it) {
		if (name[0] == *it) return(1); }
	char test2[7] = {' ', ',', '*', '?', '!', '@', '.'};
	std::vector<char> tab;
	for (int i = 0; i < 7; i ++) { tab.push_back(test2[i]); }
	for (std::vector<char>::iterator it=tab.begin(); it != tab.end(); ++it) {
		if (name.find(*it)) return(1); }
	return 0;
}
