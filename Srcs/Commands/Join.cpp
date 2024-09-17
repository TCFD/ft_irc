#include "../../Server.hpp"

void	printChanInfos(Channel chan, int idx)
{
	std::cout << "CHAN INFOS:\n";
	std::cout << MAGENTA "* Name: " << chan.gName() << std::endl;
	std::cout << "* Users: ";
	for (CLIENT_IT it = chan.gClients().begin(); it != chan.gClients().end(); ++it) {
		std::cout << it->getNickname() << " "; }
	std::cout << "\n* Limit: " << chan.gLimit() << std::endl;
	std::cout << "* Len: " << chan.gLenClients() << std::endl;
	std::cout << "* Password: " << chan.gPassword() << std::endl;
	std::cout << "* Topic: " << chan.gTopic() << std::endl;
	std::cout << "\n* NUM CHAN: " << idx << std::endl;
	std::cout << NC << std::endl;
}

void	Server::sendToEveryone(std::string msg)
{
	for (CLIENT_IT it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); ++it)
	{
		// std::cout << "IT : " << it->getNickname() << std::endl;
		_msg.response = msg;
		sendResponse(it->getFd());
	}
}

/* 
 * Handle JOIN (back up)
 * Handling of channel list, channel users, channel limit users, current channel, 
 *	  channel password or not, channel modes, channel invite-only or not
 * Useful for MODES !!
 * TO HANDLE : +l/ +k/ +i !!
*/

int  Server::join(std::string senderNick)
{
	STR_VEC split = cutModeCommand();
	int		opCodon = 0;
	if (split.size() != 2 && split.size() != 3)
		return (1);
	else if (!isChanExists(split[1]))
	{ //Creation du channel
		Channel temp(split[1]);
		temp.sName(split[1]);
		if (split.size() == 2) {
			temp.sPwd(""); } // Entree libre dans le channel
		else {
			temp.sPwd(split[2]); } // Mot de passe requis
		temp.addOperator(_clients[_msg.currentIndex]);
		temp.addClient(_clients[_msg.currentIndex]);
		temp.addLenClient();
		_channels.push_back(temp);
		opCodon = 1;
	}
	else { 
		Channel	*currChan = &_channels[_msg.currentChan];

		if (currChan->gLimit() > 0 && currChan->gLenClients() == currChan->gLimit()) {
			_msg.response = _msg.prefixNick + " 471 " + senderNick + " " + currChan->gName() + " :Cannot join channel (+l)\r\n";
			//printMessage("471", senderNick, currChan->gName() + " :Cannot join channel (+l)");
			return (1); }
		else if (currChan->gPassword() != "" && (split.size() == 2 || (split.size() == 3 && split[2] != currChan->gPassword()))) {
			_msg.response = _msg.prefixNick + " 475 " + senderNick + " " + currChan->gName() + " :Cannot join channel (+k)\r\n";
			return (1); }
		else if (foundModeInChan('i', currChan->gModes()) && !currChan->isUserInvite(senderNick)) {
			_msg.response = _msg.prefixNick + " 473 " + senderNick + " " + currChan->gName() + " :Cannot join channel (+i)\r\n";
			return (1); }
		else
		{
			if (foundModeInChan('i', currChan->gModes()))
				currChan->dltInvite(senderNick);
			currChan->addClient(_clients[_msg.currentIndex]);
			currChan->addLenClient();
		}
	}
	printChanInfos(_channels[_msg.currentChan], _msg.currentChan);
	std::cout << RED "\nNEW CHANNEL ENTERING . . . " NC << std::endl;
		
	// Loop to send the arrival of a new client to everyone in that channel
	for (CLIENT_IT it=_channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); it++)
	{
		_msg.response += ":" + senderNick + " JOIN " + _channels[_msg.currentChan].gName() + "\r\n";
		sendResponse(it->getFd());
	}
	if (opCodon == 1)
		_msg.response += _msg.prefixNick + " MODE " + _channels[_msg.currentChan].gName() + " +o " + senderNick + "\r\n";
	if (_channels[_msg.currentChan].gTopic() != "") {
		_msg.response += _msg.prefixNick + " 332 " + senderNick + " " + _channels[_msg.currentChan].gName() + " :" + _channels[_msg.currentChan].gTopic() + "\r\n";
		sendResponse(_clients[_msg.currentIndex].getFd());
		_msg.response += _msg.prefixNick + " 333 " + senderNick + " " + _channels[_msg.currentChan].gName() + " " + _channels[_msg.currentChan].gTopicName() + " " + timeToStr(std::time(0)) + "\r\n"; }
	else {
		_msg.response += _msg.prefixNick + " 331 " + senderNick + " " + _channels[_msg.currentChan].gName() + " :No topic set\r\n"; }
	sendResponse(_clients[_msg.currentIndex].getFd());

	namesHandle();
	std::cout << "MESSAGE SENT: " << _msg.response << std::endl;
	return (0);
}
