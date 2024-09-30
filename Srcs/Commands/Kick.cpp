#include "../../Includes/Server.hpp"

void	Server::kick(std::string senderNick) {

	STR_VEC cmdVec(splitCmd(_msg.command));

	//* Check if there is enough args	
	if (cmdVec.size() < 3) {
		_msg.response = print_message("461", _clients[_msg.currentIndex].getNickname(), "KICK :Not enough parameters");
		return;
	}
	
	//* L'user qui kick donne une raison
	std::string reason;
	if (_msg.command.find(':')) { 
		reason = _msg.command.substr(_msg.command.find(':'));
	}

	//* Does channel exist ? 
	CHAN_ITC targetChan = DoesChanExist(cmdVec[1]);
	if (targetChan == _channels.end()) {
		_msg.response = print_message("403", _clients[_msg.currentIndex].getNickname(), cmdVec[1] + " :No such channel");
	}

	//* Is target on channel ?
	else if (!targetChan->isUserOnMe(cmdVec[2])) {
		_msg.response = print_message("441", _clients[_msg.currentIndex].getNickname(), cmdVec[2] + " " + cmdVec[2] + " :They aren't on that channel");
	}

	//* Is channel operator ? 
	else if (!is_user_an_operator(senderNick, _channels[_msg.currentChan])) {
		_msg.response = print_message("482", senderNick, cmdVec[1] + " :You're not channel operator");
	}

	else {
		//* succesfull kick
		//* On doit envoyer le message :sender KICK #channel @user a tous les users connectes aux channels
		sendToChan(":" + senderNick + " KICK " + cmdVec[1] + " " + cmdVec[2] + " " + reason +"\n");
		//* Gestion/mise a jour de nos donnees
		_channels[_msg.currentChan].dltClient(cmdVec[2]);
		_channels[_msg.currentChan].dltOperator(cmdVec[2]);

		int userFd = getFdOfUser(cmdVec[2]);
		if (userFd != -1)
			sendResponse(userFd);
	}
}