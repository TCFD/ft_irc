#include "../../Includes/Server.hpp"

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

bool	Server::errorsTopic(STR_VEC split, Client *currentUser)
{
	Channel	*current = &_channels[_msg.currentChan];

	if (split.size() == 1) {
		_msg.response = _msg.prefixNick + " 461 " + currentUser->getNickname() + " TOPIC :Not enough parameters\r\n"; }
	else if ((split[1].find("#") != std::string::npos && !isChanExists(split[1])) || current->gName().empty()) {
		_msg.response = _msg.prefixNick + " 403 " + currentUser->getNickname() + " " + split[1] + " :No such channel\r\n"; }
	else if (isChanExists(split[1]) && !is_user_in_chan(currentUser->getNickname(), current)) {
		_msg.response = _msg.prefixNick + " 442 " + currentUser->getNickname() + " " + split[1] + " :You're not on that channel\r\n"; }
	else if (found_mode_in_chan('t', current->gModes()) && !is_user_an_operator(currentUser->getNickname(), split[1])) {
		_msg.response = _msg.prefixNick + " 482 " + currentUser->getNickname() + " " + current->gName() + " :You're not channel operator\r\n"; }
	else
		return false;
	sendResponse(currentUser->getFd());
	return true;
}

void	Server::topicHandle(Client *currentUser)
{
	STR_VEC split = cutTopicCmd();
	Channel	*current = &_channels[_msg.currentChan];

	std::cout << "Topic command :\n";
	for (STR_VEC::iterator it = split.begin(); it != split.end(); it++)
		std::cout << "[" << *it << "]";
	std::cout << std::endl;
	if (!errorsTopic(split, currentUser))
	{
		current->sTopic(split[2]);
		current->sTopicName(currentUser->getNickname());
		sendToEveryone(_msg.prefixNick + " TOPIC " + current->gName() + " " + current->gTopic() + "\r\n");
	}
}
