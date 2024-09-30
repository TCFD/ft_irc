#include "../../Includes/Server.hpp"

void	Server::kick(std::string senderNick) {

	STR_VEC cmdVec(splitCmd(_msg.command));

	//* Check if there is enough args	
	if (cmdVec.size() < 3) {
		_msg.response = print_message("461", _clients[_msg.currentIndex].getNickname(), "KICK :Not enough parameters");
		return;
	}
	
	std::string reason;
	if (_msg.command.find(':')) { //* L'user qui kick donne une raison
		reason = _msg.command.substr(_msg.command.find(':'));
	}

	//if (cmdVec.size() > 4 || (cmdVec.size() > 3 && cmdVec[3][0] != ':')) { 
		//* L'user veut kick plus d'une personne
		//* On va faire tourner une fonction recursive pour kick les users 1 par 1
		//* Avant ca on doit verifier qu'il y a soit autant d'users que de channels (il peut donc avoir un user qui va etre kick de #a et un autre de #b)
		//* Ou, il y a un seul channel. Dans ce cas la, on kick tous les users fournis par la commande depuis ce meme channel
		//* Pour que ca soit plus simple, on va compter le nombre de virgules pour les channels et pour les users. 0 virgules = 1 channel.


		//? Est ce qu'on doit le faire ? Irssi est un client intelligent et donc pour simplifier les choses,
		//? il envoie simplement deux commandes KICK differentes plutot qu'une seule avec plusieurs users
		//for (STR_VEC::iterator it = cmdVec.begin(); it < cmdVec.end(); it++)
		//	std::cout << *it << "\n";

		
	//}
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