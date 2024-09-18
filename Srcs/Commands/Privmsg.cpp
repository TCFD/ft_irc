#include "../../Server.hpp"

void	Server::privmsg(std::string senderNick)
{
	std::string destinataire = _msg.command.substr(8, _msg.command.find(':') - 9);
	std::string msg = _msg.command.substr(_msg.command.find(':'));

	std::cout << "destinataire : \'" << destinataire << "\'\n";
	if (destinataire.empty())
	{
		_msg.response = print_message("401", _clients[_msg.currentIndex].getNickname(), " :No such nick/channel");
		return;
	}
	
	if (msg.empty())
	{
		_msg.response = print_message("412", _clients[_msg.currentIndex].getNickname(), " :No text to send");
		return;
	}
	
	if (destinataire[0] == '#')
	{ //* On envoie vers un channel
		CHAN_IT targetChan = DoesChanExist(destinataire);
		if (targetChan == _channels.end()) {
			_msg.response = print_message("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel");
			return;
		}
		CLIENT_VEC clients = targetChan->gClients();
		CLIENT_IT it = clients.begin();
		for (; it < clients.end(); it++) {
			_msg.response = ":" + senderNick + "!~" + senderNick + "@host PRIVMSG " + destinataire + " :" + msg.substr(1) + "\r\n";
			if (it->getNickname() != senderNick) {
				std::cout << "Sending msg to " << it->getNickname() << std::endl;
				sendResponse(it->getFd());
			}
		}
		_msg.response = "";
		return ;
	}
	else { //* On envoie vers un user
		int targetUser = getFdOfUser(destinataire);
		if (targetUser == -1) {
			_msg.response = print_message("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel");
			return; }
		_msg.response = ":" + senderNick + "!~" + senderNick + "@host PRIVMSG " + destinataire + " :" + msg.substr(1) + "\r\n";
		sendResponse(targetUser);
	}
}