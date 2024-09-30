#include "../../Includes/Server.hpp"

/* USER registration */
void    Server::userHandle(Client   *currentUser)
{
    if (currentUser->getPasswd() && !currentUser->getRegistered()) {
		currentUser->setUsername(_msg.command.substr(5, _msg.command.find(" ", 5) - 5));
		currentUser->setRealname(_msg.command.substr(_msg.command.find(":")));
		if (currentUser->getNickname() != "") {
			currentUser->setRegistered(true);
			currentUser->setId(currentUser->getNickname() + "!" + currentUser->getUsername() + "@" + currentUser->getHostname());
			_msg.response = print_message("001", currentUser->getNickname(), ":Welcome to the Internet Relay Network " + currentUser->getId());
		}
	}
	else if (currentUser->getRegistered())
		_msg.response = print_message("462", currentUser->getNickname(), ":Unauthorized command (already registered)");
	else
	{
		_msg.response = ":server Please set your password first\r\n";
		sendResponse(currentUser->getFd());
		_msg.response += ":server KILL * :Invalid password, please retry.\r\n";
		sendResponse(currentUser->getFd());
	}
}