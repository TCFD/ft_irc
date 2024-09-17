#include "../../Server.hpp"

void    	Server::pass(Client *currenUser)
{
    STR_VEC cmdVec(splitCmd(_msg.command));
    std::string currNick = _clients[_msg.currentIndex].getNickname();

    if (cmdVec.empty()) {
        _msg.response = printMessage("461", "", " PASS :Not enough parameters");}

    else if (cmdVec[1] != _mdp) {
        _msg.response = printMessage("464", "", ":Password incorrect");
		sendResponse(currenUser->getFd());
		_msg.response += ":server KILL :invalid password \n\r";
		// _poll.disconnectClient(i, server);
		// close(_serverSocket);
	}
    else
        std::cout << "C'est good mec !\n";
}