#include "Server.hpp"

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
			std::cout << "split: " << split[j] << std::endl;
			break ; }
		else
		{
			split.push_back(_msg.command.substr(0, _msg.command.find(delim)));
			_msg.command.erase(0, (int)split[j].size() +1); 
			std::cout << "split: " << split[j] << std::endl; }
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

std::string	getTime(void)
{
	std::time_t currentTime = std::time(0);
	std::cout << CYAN "Time: " << currentTime << NC << std::endl;
	std::tm*	localTime = std::localtime(&currentTime);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", localTime);
	return std::string(buffer); 
}

void	Server::topicHandle(void)
{
	STR_VEC split = cutTopicCmd();
	Channel	*current = &_channels[_msg.currentChan];

	std::string buff = getTime();
	if (!errorsTopic(split))
	{
		current->sTopic(split[2]);
		std::cout << GREEN "TOPIC is now set: " << current->gTopic() << " on " << buff << NC << std::endl;
		sendToEveryone(_msg.prefixNick + " 332 " + _clients[_msg.currentIndex].getNickname() + " " + current->gName() + " :" + current->gTopic() + "\r\n");
	}
}
