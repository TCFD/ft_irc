#include "../../Includes/Server.hpp"

CHAN_IT Server::DoesChanExist (std::string target) {
	CHAN_IT it = _channels.begin();

	for (; it != _channels.end(); it++) {
		if (it->gName() == target) {
			return it;
		}
	}
	return it;
}

// returns the fd of asked user, -1 if not found
int	Server::getFdOfUser(std::string nick) {
	CLIENT_IT it = _clients.begin();

	for (; it != _clients.end(); it++) {
		if (it->getNickname() == nick)
			return (it->getFd());
	}
	return -1;
}

void	Server::addClienttoInviteList(std::string name) {
	for (CLIENT_IT it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickname() == name)
			_channels[_msg.currentChan].addInvite(*it);
	}
}

void	Server::invite(std::string senderNick) {
	STR_VEC cmdVec(splitCmd(_msg.command));

	//* Check if there is enough args	
	if (cmdVec.size() < 3) {
		_msg.response = print_message("461", senderNick, "INVITE :Not enough parameters");
		return;
	}
	//* Does channel exist ? 
	CHAN_IT targetChan = DoesChanExist(cmdVec[2]);
	if (targetChan == _channels.end()) {
		_msg.response = print_message("403", senderNick, cmdVec[2] + " :No such channel");
		return;
	}
	//* Is user exists ?
	if (!is_user_exists(cmdVec[1], _clients)) {
		_msg.response = print_message("401", senderNick, cmdVec[1] + " :No suck nick");
		return;
	};

	//* Is user on channel ?
	if (!targetChan->isUserOnMe(senderNick)) {
		_msg.response = print_message("442", senderNick, cmdVec[2] + " :You're not on that channel");
		return;
	}
	//* Is target on channel ?
	if (targetChan->isUserOnMe(cmdVec[1])) {
		_msg.response = print_message("443", senderNick, cmdVec[1] + " " + cmdVec[2] + " :Is already on channel");
		return;
	}

	//* Successfull invite
	int userFd = getFdOfUser(cmdVec[1]);
	if (userFd != -1) {
		_msg.response = print_message("341", senderNick, cmdVec[1] + " " + cmdVec[2]);
		sendResponse(userFd);
		_msg.response = std::string(BOLD) + "Whats up " + std::string(BLUE) + cmdVec[1] + std::string(NC) + std::string(BOLD) + ", " + std::string(BLUE) + senderNick + std::string(NC) + std::string(BOLD) + " invited you to channel " + std::string(BLUE) + cmdVec[2] + std::string(NC) + "\r\n";
		sendResponse(userFd);
		addClienttoInviteList(cmdVec[1]);
	}
	_msg.response = print_message("341", senderNick, cmdVec[1] + " " + cmdVec[2]);
}	