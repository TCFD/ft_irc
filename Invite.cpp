#include "Server.hpp"

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

void	Server::invite() {
	STR_VEC cmdVec(splitCmd(_msg.command));

	//* Check if there is enough args	
	if (cmdVec.size() < 3) {
		_msg.response = printMessage("461", _clients[_msg.currentIndex].getNickname(), "Invite :Not enough parameters");
		return;
	}
	//* Does channel exist ? 
	std::cout <<  "does channel exist\n";
	CHAN_IT targetChan = DoesChanExist(cmdVec[2]);
	if (targetChan == _channels.end()) {
		_msg.response = printMessage("403", _clients[_msg.currentIndex].getNickname(), cmdVec[2] + " :No such channel");
		return;
	}
	//* Is user on channel ?
	if (!targetChan->isUserOnMe(_clients[_msg.currentIndex].getNickname())) {
		_msg.response = printMessage("442", _clients[_msg.currentIndex].getNickname(), cmdVec[2] + " :You're not on that channel");
		return;
	}
	//* Is target on channel ?
	if (targetChan->isUserOnMe(cmdVec[1])) {
		_msg.response = printMessage("443", _clients[_msg.currentIndex].getNickname(), cmdVec[1] + " " + cmdVec[2] + " :is already on channel");
		return;
	}

	//* Successfull invite
	//TODO On doit aussi envoyer au client invite
	int userFd = getFdOfUser(cmdVec[1]);
	if (userFd != -1) {
		_msg.response = printMessage("341", _clients[_msg.currentIndex].getNickname(), cmdVec[1] + " " + cmdVec[2]);
		sendResponse(userFd);
		_msg.response = std::string(BOLD) + "Whats up " + std::string(BLUE) + cmdVec[1] + std::string(NC) + std::string(BOLD) + ", " + std::string(BLUE) + _clients[_msg.currentIndex].getNickname() + std::string(NC) + std::string(BOLD) + " invited you to channel " + std::string(BLUE) + cmdVec[2] + std::string(NC) + "\r\n";
		sendResponse(userFd);

	}
	_msg.response = printMessage("341", _clients[_msg.currentIndex].getNickname(), cmdVec[1] + " " + cmdVec[2]);
}