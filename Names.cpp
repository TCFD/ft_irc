#include "Server.hpp"

// Print all the names from the current channel
// Si on est dans le chan: Max 1 argument ou 2 si le 2e est le nom du chan
// Si on n'est pas dans le chan: 2 arguments, /names #chan

	//NAMES a revoir: le message ne marche pas et jsp pourquoi
		
void			Server::namesHandle(void)
{
	STR_VEC split = cutModeCommand();
	// std::cout << "SPLIT: " << split[1] << "\nsuis je ici ?\n";
	if (_msg.currentChan != -1)
	{
		if (split.size() <= 1 || (split.size() == 2 && split[1] == _channels[_msg.currentChan].gName()))
		{
			Channel *chan = &_channels[_msg.currentChan];
			std::string nicks = "";
			for (CLIENT_IT it = chan->gClients().begin(); it != chan->gClients().end(); it++)
			{
				nicks += it->getNickname();
			}
			sendToEveryone(_msg.prefixNick + " NAMES " + _channels[_msg.currentChan].gName() + "\r\n");
			// _msg.response += ":server 353 " + _clients[_msg.currentIndex].getNickname() + " " + _channels[_msg.currentChan].gName() + " :" + nicks + "\r\n";
		}

	}
}
