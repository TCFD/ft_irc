#include "../../Includes/Server.hpp"

void    	Server::pass(Client *currenUser)
{
    STR_VEC cmdVec(splitCmd(_msg.command));
    std::string currNick = _clients[_msg.currentIndex].getNickname();

	if (currenUser->getRegistered())
		_msg.response = print_message("462", currenUser->getNickname(), ":Unauthorized command (already registered)");
    else if (cmdVec.empty()) {
        _msg.response = print_message("461", "", " PASS :Not enough parameters");
		sendResponse(currenUser->getFd());
		_msg.response += ":server KILL * : Invalid password, please retry.\r\n";
		}

    else if (cmdVec[1] != _mdp) {
        _msg.response = print_message("464", "", ":Password incorrect");
		sendResponse(currenUser->getFd());
		_msg.response += ":server KILL * : Invalid password, please retry.\r\n";
	}
    else
        currenUser->setPasswd(true);
}