#include "Server.hpp"

STR_VEC    Server::cutTopicCmd(void)
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



void    Server::topicHandle(void)
{
    STR_VEC split = cutTopicCmd();
    if (split.size() > 2)
    {
        _channels[_msg.currentChan].sTopic(split[2]);
        std::cout << GREEN "TOPIC is now set: " << _channels[_msg.currentChan].gTopic() << NC << std::endl;
        sendToEveryone(_msg.prefixNick + " 332 " + _clients[_msg.currentIndex].getNickname() + " " + _channels[_msg.currentChan].gName() + " :" + _channels[_msg.currentChan].gTopic() + "\r\n");
    }
}
