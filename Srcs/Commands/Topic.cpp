#include "../../Server.hpp"

STR_VEC	Server::cutTopicCmd(void)
{
	std::string delim = " ";
	STR_VEC split;
	for (size_t j=0; _msg.command.length() != 0; j++)
	{
		if (j == 2)
		{
			split.push_back(_msg.command.substr(1));
			_msg.command.erase(0, (int)split[j].size() +1);
			break ; }
		else
		{
			split.push_back(_msg.command.substr(0, _msg.command.find(delim)));
			_msg.command.erase(0, (int)split[j].size() +1); }
	}
	return split;
}

bool	Server::errorsTopic(STR_VEC split)
{
	Channel	*current = &_channels[_msg.currentChan];

	if (split.size() == 1 && _msg.currentChan == 0) {
		_msg.response = _msg.prefixNick + " 461 " + _clients[_msg.currentIndex].getNickname() + " TOPIC :Not enough parameters\r\n"; }
	else if (!(split[1].find("#")) && !isChanExists(split[1])) {
		_msg.response = _msg.prefixNick + " 403 " + _clients[_msg.currentIndex].getNickname() + " " + split[1] + " :No such channel\r\n"; }
	else if (!(split[1].find("#")) && isChanExists(split[1]) && !isUserInChan(_clients[_msg.currentIndex].getNickname(), *current)) {
		_msg.response = _msg.prefixNick + " 442 " + _clients[_msg.currentIndex].getNickname() + " " + split[1] + " :You're not on that channel\r\n"; }
	else if (foundModeInChan('t', current->gModes()) && !isUserAnOperator(_clients[_msg.currentIndex].getNickname(), *current)) {
		_msg.response = _msg.prefixNick + " 482 " + _clients[_msg.currentIndex].getNickname() + " " + current->gName() + " :You're not channel operator\r\n"; }
	else
		return false;
	return true;
}

void	Server::topicHandle(void)
{
	STR_VEC split = cutTopicCmd();
	Channel	*current = &_channels[_msg.currentChan];

	if (!errorsTopic(split))
	{
		current->sTopic(split[2]);
		current->sTopicName(_clients[_msg.currentIndex].getNickname());
		sendToEveryone(_msg.prefixNick + " TOPIC " + current->gName() + " " + current->gTopic() + "\r\n");
	}
}
