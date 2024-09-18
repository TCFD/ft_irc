#include "../../Server.hpp"

// Pour envoyer un privmsg vers un channel

void	Server::send_primsg_to_channel(std::string senderNick, std::string destinataire, std::string msg)
{
	CHAN_IT targetChan = DoesChanExist(destinataire);

	if (targetChan == _channels.end())
		return (setMsgReponse(printMessage("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel")));
		//_msg.response = printMessage("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel");
	
	CLIENT_VEC clients = targetChan->gClients();
	CLIENT_IT it = clients.begin();
	for (; it < clients.end(); it++)
	{
		_msg.response = ":" + senderNick + "!~" + senderNick + "@host PRIVMSG " + destinataire + " :" + msg.substr(1) + "\r\n";
		if (it->getNickname() != senderNick)
		{
			std::cout << "Sending msg to " << it->getNickname() << std::endl;
				sendResponse(it->getFd(), it->getNickname());
		}
	}
	_msg.response = "";
	return ;
}

// De même que [send_primsg_to_channel] à la différence qu'on envoie vers un user

void	Server::send_privmsg_to_user(std::string senderNick, std::string destinataire, std::string msg)
{
	int targetUser = getFdOfUser(destinataire);

	if (targetUser == -1)
		return (setMsgReponse(printMessage("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel")));
		//_msg.response = printMessage("401", _clients[_msg.currentIndex].getNickname(), destinataire + " :No such nick/channel");
	
	_msg.response = ":" + senderNick + "!~" + senderNick + "@host PRIVMSG " + destinataire + " :" + msg.substr(1) + "\r\n";
	sendResponse(targetUser, destinataire);
}


void	Server::privmsg(std::string senderNick)
{
	std::string destinataire = _msg.command.substr(8, _msg.command.find(':') - 9);
	std::string msg = _msg.command.substr(_msg.command.find(':'));

	std::cout << "destinataire : \'" << destinataire << "\'\n";
	if (destinataire.empty())
		return (setMsgReponse(printMessage("401", _clients[_msg.currentIndex].getNickname(), " :No such nick/channel")));
		//_msg.response = printMessage("401", _clients[_msg.currentIndex].getNickname(), " :No such nick/channel");
	
	if (msg.empty())
		return (setMsgReponse(printMessage("412", _clients[_msg.currentIndex].getNickname(), " :No text to send")));
		//_msg.response = printMessage("412", _clients[_msg.currentIndex].getNickname(), " :No text to send");
	
	// Envoie vers un channel
	if (destinataire[0] == '#')
		return (send_primsg_to_channel(senderNick, destinataire, msg));

	// Else, Envoie vers un user
	send_privmsg_to_user(senderNick, destinataire, msg);
}
