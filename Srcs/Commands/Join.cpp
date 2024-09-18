#include "../../Includes/Server.hpp"

void	printChanInfos(Channel chan, int idx)
{
	std::cout << "\nCHAN INFOS:\n";
	std::cout << MAGENTA "- Name: " << chan.gName() << std::endl;
	std::cout << "- Users: ";
	for (CLIENT_IT it = chan.gClients().begin(); it != chan.gClients().end(); ++it) {
		std::cout << it->getNickname() << " "; }
	std::cout << "\n- Limit: " << chan.gLimit() << std::endl;
	std::cout << "- Nb of users: " << chan.gLenClients() << std::endl;
	if (chan.gPassword() != "")
		std::cout << "- Password: " << chan.gPassword() << std::endl;
	if (chan.gTopic() != "")
		std::cout << "- Topic: " << chan.gTopic() << std::endl;
	std::cout << "- NUM CHAN: " << idx << std::endl;
	if (chan.gPassword() == "" && chan.gTopic() == "")
	std::cout << "- This channel has no password and no topic." << std::endl;
	else if (chan.gTopic() == "")
		std::cout << "- This channel has no topic." << std::endl;
	else if (chan.gPassword() == "" )
	std::cout << "- This channel has no password." << std::endl;

	std::cout << NC << std::endl;
}

void	Server::sendToEveryone(std::string msg)
{
	for (CLIENT_IT it = _channels[_msg.currentChan].gClients().begin(); it != _channels[_msg.currentChan].gClients().end(); ++it)
	{
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
	{
		Channel temp(split[1]);
		temp.sName(split[1]);
		if (split.size() == 2) {
			temp.sPwd(""); }
		else {
			temp.sPwd(split[2]); }
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
			return (1); }
		else if (currChan->gPassword() != "" && (split.size() == 2 || (split.size() == 3 && split[2] != currChan->gPassword()))) {
			_msg.response = _msg.prefixNick + " 475 " + senderNick + " " + currChan->gName() + " :Cannot join channel (+k)\r\n";
			return (1); }
		else if (found_mode_in_chan('i', currChan->gModes()) && !currChan->isUserInvite(senderNick)) {
			_msg.response = _msg.prefixNick + " 473 " + senderNick + " " + currChan->gName() + " :Cannot join channel (+i)\r\n";
			return (1); }
		else
		{
			if (found_mode_in_chan('i', currChan->gModes()))
				currChan->dltInvite(senderNick);
			currChan->addClient(_clients[_msg.currentIndex]);
			currChan->addLenClient();
		}
	}
	printChanInfos(_channels[_msg.currentChan], _msg.currentChan);
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
		_msg.response += _msg.prefixNick + " 333 " + senderNick + " " + _channels[_msg.currentChan].gName() + " " + _channels[_msg.currentChan].gTopicName() + " " + time_to_str(std::time(0)) + "\r\n"; }
	else {
		_msg.response += _msg.prefixNick + " 331 " + senderNick + " " + _channels[_msg.currentChan].gName() + " :No topic set\r\n"; }
	sendResponse(_clients[_msg.currentIndex].getFd());

	namesHandle();
	return (0);
}
