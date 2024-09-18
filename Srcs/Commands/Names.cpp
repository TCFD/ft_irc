#include "../../Server.hpp"

// Print all the names from the current channel
// Si on est dans le chan: Max 1 argument ou 2 si le 2e est le nom du chan
// Si on n'est pas dans le chan: 2 arguments, /names #chan

void			Server::namesHandle(void)
{
	STR_VEC split = cutModeCommand();
	if (split.size() <= 1)
	{
		Channel *chan = &_channels[_msg.currentChan];
		std::string nicks = "";
		for (CLIENT_IT it = chan->gClients().begin(); it != chan->gClients().end(); it++){
		for (CLIENT_IT ite = chan->gOperators().begin(); ite != chan->gOperators().end(); ++ite) {
			if (it->getNickname() == ite->getNickname())
				nicks += "@"; }
		nicks += it->getNickname() + " "; }
		_msg.response = _msg.prefixNick + " 353 " + _clients[_msg.currentIndex].getNickname() + " = " + chan->gName() + " :" + nicks + "\r\n";
		_msg.response += _msg.prefixNick + " 366 " + _clients[_msg.currentIndex].getNickname() + " " + chan->gName() + " :End of /NAMES list\r\n";
	}
}
